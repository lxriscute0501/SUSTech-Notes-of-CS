#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
GSM8K evaluator (vLLM)
Supports:
 - zero-shot CoT evaluation
 - few-shot evaluation with k-shot exemplars (NO chain-of-thought in exemplars)
Features in this file:
 - unified prompt template for zero/few
 - few-shot exemplars include only final answer (#### N)
 - progress reporting: prints "[processed / total] processed" during run
 - final concise output: prints only "Accuracy: XX.XX%" after each evaluation run
 - CPU-friendly defaults: tensor_parallel_size=1, device auto (can pass --device cpu)
Notes:
 - This script expects vllm to be installed and available.
 - For CPU runs, use --tensor_parallel_size 1 and --device cpu (and consider setting
   environment variable VLLM_CPU_KVCACHE_SPACE to a suitable GiB value).
"""

from __future__ import annotations

import argparse
import re
import sys
from fractions import Fraction
from typing import List, Tuple

import jsonlines
from vllm import LLM, SamplingParams

MAX_INT = sys.maxsize


def is_number(s: str) -> bool:
    try:
        float(s)
        return True
    except Exception:
        pass
    try:
        import unicodedata

        unicodedata.numeric(s)
        return True
    except Exception:
        pass
    return False


def extract_answer_number(completion: str):
    """
    Try multiple heuristics to extract a final numeric answer from model completion.
    - Prefer explicit patterns: 'The answer is:', 'Final answer:', '#### <num>'
    - Support integer, decimal, fraction (a/b)
    - Fallback: last numeric token in text
    Returns: rounded number (int) or None
    """
    if completion is None:
        return None
    text = completion

    # 1) explicit delimiters
    patterns = [
        r'(?:The answer is|The final answer is|Final answer is|Answer:)\s*([\-+]?\d+[,\d\.\/]*)',
        r'####\s*([\-+]?\d+[,\d\.\/]*)',
        r'Answer\s*is\s*[:\-]?\s*([\-+]?\d+[,\d\.\/]*)',
    ]
    for pat in patterns:
        m = re.search(pat, text, flags=re.IGNORECASE)
        if m:
            token = m.group(1)
            token = token.replace(',', '')
            # fraction?
            if '/' in token:
                parts = token.split('/')
                if len(parts) == 2 and is_number(parts[0]) and is_number(parts[1]):
                    denom = parts[1]
                    if denom == '0':
                        try:
                            return round(float(parts[0]))
                        except Exception:
                            return None
                    try:
                        frac = Fraction(parts[0]) / Fraction(parts[1])
                        return round(float(frac))
                    except Exception:
                        try:
                            return round(float(Fraction(token)))
                        except Exception:
                            return None
            else:
                try:
                    val = float(token)
                    if val == float('inf') or val == float('-inf'):
                        return None
                    return round(val)
                except Exception:
                    pass

    # 2) fallback: find last numeric-like token in text
    matches = re.findall(r'[\-+]?\d+[,\d\.\/]*', text)
    if matches:
        last = matches[-1].replace(',', '')
        try:
            if '/' in last:
                # fraction fallback
                frac = Fraction(last)
                return round(float(frac))
            else:
                val = float(last)
                if val == float('inf') or val == float('-inf'):
                    return None
                return round(val)
        except Exception:
            return None

    return None


def chunk_list(lst: List, chunk_size: int) -> List[List]:
    """Proper chunking of a list into chunks of size <= chunk_size."""
    if chunk_size <= 0:
        return [lst[:]]
    return [lst[i:i + chunk_size] for i in range(0, len(lst), chunk_size)]


def load_gsm8k(data_path: str) -> Tuple[List[dict], List[int]]:
    """
    Load GSM8K jsonlines file. Each line expected to contain at least:
      - 'query' (question text)
      - 'response' (solution text that includes answer like '#### 42')
    Returns:
      - dataset: list of dicts (original items)
      - answers: list of integer answers extracted from the response field
    """
    dataset = []
    answers = []
    with open(data_path, "r", encoding="utf8") as f:
        reader = jsonlines.Reader(f)
        for item in reader:
            dataset.append(item)
            # try extract ground-truth answer from 'response'
            resp = item.get("response", "")
            # common format: contains '#### 42' near end
            m = re.search(r'####\s*([\-+]?\d+[,\d\.\/]*)', resp)
            if m:
                token = m.group(1).replace(',', '')
                try:
                    if '/' in token:
                        answers.append(round(float(Fraction(token))))
                    else:
                        answers.append(int(float(token)))
                except Exception:
                    # fallback: try to find last integer
                    mm = re.findall(r'[\-+]?\d+[,\d\.\/]*', resp)
                    if mm:
                        t = mm[-1].replace(',', '')
                        try:
                            if '/' in t:
                                answers.append(round(float(Fraction(t))))
                            else:
                                answers.append(int(float(t)))
                        except Exception:
                            answers.append(None)
                    else:
                        answers.append(None)
            else:
                # fallback: search last number in response
                mm = re.findall(r'[\-+]?\d+[,\d\.\/]*', resp)
                if mm:
                    t = mm[-1].replace(',', '')
                    try:
                        if '/' in t:
                            answers.append(round(float(Fraction(t))))
                        else:
                            answers.append(int(float(t)))
                    except Exception:
                        answers.append(None)
                else:
                    answers.append(None)
    return dataset, answers


# unified template
PROBLEM_PROMPT = (
    "Below is an instruction that describes a task. "
    "Write a response that appropriately completes the request.\n\n"
    "### Instruction:\n{instruction}\n\n### Response: Let's think step by step."
)


def build_zero_shot_prompt(question: str) -> str:
    return PROBLEM_PROMPT.format(instruction=question)


def build_few_shot_prompt(exemplars: List[dict], question: str) -> str:
    """
    Construct a few-shot prompt where exemplars do NOT include chain-of-thought.
    Each exemplar includes only the question and final numeric answer in the form:
      ### Instruction:
      {query}
      ### Response:
      #### {answer}
    Finally append the target question using the PROBLEM_PROMPT (which includes "Let's think step by step.")
    """
    pieces = []
    for ex in exemplars:
        q = ex.get("query", "").strip()
        resp = ex.get("response", "").strip()
        # extract only the final '#### N' token from resp
        m = re.search(r'####\s*([\-+]?\d+[,\d\.\/]*)', resp)
        if not m:
            # if exemplar has no clear final answer, skip it
            continue
        ans_token = m.group(0)  # e.g., "#### 42"
        exemplar_text = (
            "Below is an instruction that describes a task. "
            "Write a response that appropriately completes the request.\n\n"
            "### Instruction:\n{instruction}\n\n### Response:\n{final}"
        ).format(instruction=q, final=ans_token)
        pieces.append(exemplar_text)
    # append the target question (with CoT "Let's think step by step.")
    target = PROBLEM_PROMPT.format(instruction=question)
    pieces.append(target)
    return "\n\n".join(pieces)


def evaluate_prompts(
    llm_model_path: str,
    prompts: List[str],
    gold_answers: List[int],
    batch_size: int = 1,
    tensor_parallel_size: int = 1,
    max_tokens: int = 512,
    device: str = "auto",
    stop_tokens: List[str] | None = None,
):
    """
    Evaluate prompts with vLLM, printing periodic progress and returning accuracy.
    Prints progress lines like: "[{processed} / {total}] processed"
    At the end prints exactly one concise accuracy line: "Accuracy: XX.XX%"
    Returns: (accuracy_float, invalid_outputs_list)
    """
    if stop_tokens is None:
        stop_tokens = ["Question:", "Question", "USER:", "USER", "ASSISTANT:", "ASSISTANT", "Instruction:", "Instruction"]
    sampling_params = SamplingParams(temperature=0.0, top_p=1, max_tokens=max_tokens, stop=stop_tokens)

    # Create LLM once per evaluation run
    llm_kwargs = {"model": llm_model_path, "tensor_parallel_size": tensor_parallel_size}
    # pass device if supported by vLLM LLM class (common arg)
    try:
        llm = LLM(**llm_kwargs, device=device)
    except TypeError:
        # fallback if LLM doesn't accept device kwarg
        llm = LLM(**llm_kwargs)

    total = len(prompts)
    if total == 0:
        print("Accuracy: 0.00%")
        return 0.0, []

    processed = 0
    correct = 0
    invalid_outputs = []

    # iterate in batches
    for i in range(0, total, batch_size):
        batch_prompts = prompts[i: i + batch_size]
        batch_golds = gold_answers[i: i + batch_size]

        # vLLM generate: returns an iterable of generation results in same order
        completions = llm.generate(batch_prompts, sampling_params)

        # extract outputs and compare
        # note: completions is iterable; iterate to collect sequentially
        local_idx = 0
        for output in completions:
            # safety: ensure we don't run past batch_golds
            if local_idx >= len(batch_golds):
                break
            generated_text = output.outputs[0].text
            gold = batch_golds[local_idx]
            pred = extract_answer_number(generated_text)
            if pred is not None and gold is not None:
                if float(pred) == float(gold):
                    correct += 1
                else:
                    # incorrect
                    pass
            else:
                # failed to extract or no gold -> count as incorrect and record
                invalid_outputs.append({
                    "index": i + local_idx,
                    "prompt": batch_prompts[local_idx],
                    "output": generated_text,
                    "pred_extracted": pred,
                    "gold": gold
                })
            local_idx += 1

        processed = min(i + batch_size, total)
        # progress line (keeps user informed)
        print(f"[{processed} / {total}] processed")

    accuracy = (correct / total) * 100.0
    # final concise accuracy line
    print(f"Accuracy: {accuracy:.2f}%")
    return accuracy, invalid_outputs


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", type=str, required=True, help="Path or HF id of vLLM-compatible model (e.g., Qwen/Qwen2.5-0.5B)")
    parser.add_argument("--data_file", type=str, required=True, help="GSM8K jsonlines file")
    parser.add_argument("--mode", type=str, default="both", choices=["zero", "few", "both"], help="Evaluation mode")
    parser.add_argument("--start", type=int, default=0, help="Start index (inclusive)")
    parser.add_argument("--end", type=int, default=MAX_INT, help="End index (exclusive)")
    parser.add_argument("--batch_size", type=int, default=1, help="Number of prompts per batch (use small on CPU)")
    parser.add_argument("--tensor_parallel_size", type=int, default=1, help="vLLM tensor_parallel_size (use 1 on CPU)")
    parser.add_argument("--k_shot", type=int, default=5, help="k for few-shot (number of exemplars)")
    parser.add_argument("--max_tokens", type=int, default=512, help="max tokens to generate")
    parser.add_argument("--device", type=str, default="auto", help="Device for vLLM (e.g., 'cpu' or 'cuda')")
    args = parser.parse_args()

    dataset, answers = load_gsm8k(args.data_file)
    # cut by start/end
    dataset = dataset[args.start: min(args.end, len(dataset))]
    answers = answers[args.start: min(args.end, len(answers))]
    dataset = dataset[:132]
    answers = answers[:132]
    n = len(dataset)
    print(f"Loaded {n} samples from {args.data_file} (indices {args.start}:{args.end})")

    # prepare zero-shot prompts
    zero_prompts = [ build_zero_shot_prompt(item.get("query", "")) for item in dataset ]

    # evaluate zero-shot if requested
    if args.mode in ("zero", "both"):
        print("==== Running zero-shot CoT evaluation ====")
        acc_zero, invalid_zero = evaluate_prompts(
            llm_model_path=args.model,
            prompts=zero_prompts,
            gold_answers=answers,
            batch_size=args.batch_size,
            tensor_parallel_size=args.tensor_parallel_size,
            max_tokens=args.max_tokens,
            device=args.device
        )
        # Note: evaluate_prompts already prints final "Accuracy: xx.xx%"

    # prepare few-shot prompts (use first k exemplars from dataset as demonstrations)
    if args.mode in ("few", "both"):
        k = args.k_shot
        if len(dataset) <= k:
            raise ValueError(f"Dataset too small for k_shot={k}")
        exemplars = dataset[:k]
        few_prompts = []
        # Build few-shot prompt per sample (we keep consistent behavior described earlier)
        for i, item in enumerate(dataset):
            q = item.get("query", "")
            p = build_few_shot_prompt(exemplars, q)
            few_prompts.append(p)

        print(f"==== Running few-shot evaluation (k={k}, no CoT in exemplars) ====")
        acc_few, invalid_few = evaluate_prompts(
            llm_model_path=args.model,
            prompts=few_prompts,
            gold_answers=answers,
            batch_size=args.batch_size,
            tensor_parallel_size=args.tensor_parallel_size,
            max_tokens=args.max_tokens,
            device=args.device
        )
        # evaluate_prompts prints final accuracy for few-shot run as well


if __name__ == "__main__":
    main()
