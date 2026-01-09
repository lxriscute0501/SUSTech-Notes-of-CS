#!/usr/bin/env python3
import argparse
import json
import pdb
import jsonlines
import random
import sys

import util
from vllm import LLM, SamplingParams

MAX_INT = sys.maxsize
INVALID_ANS = "[invalid]"

invalid_outputs = []


# -------------------------
# Utilities
# -------------------------
def remove_boxed(s):
    left = "\\boxed{"
    try:
        assert s[:len(left)] == left
        assert s[-1] == "}"
        return s[len(left):-1]
    except:
        return None


def process_results(doc, completion, answer):
    split_ans = completion.split('The answer is: ')
    if len(split_ans) > 1:
        ans = split_ans[-1]
        extract_ans_temp = ans.split('.\n')[0]
        extract_ans_temp = extract_ans_temp.strip()
        if len(extract_ans_temp)>0 and extract_ans_temp[-1] == '.':
            extract_ans = extract_ans_temp[0:-1]
        else:
            extract_ans = extract_ans_temp
        extract_ans = extract_ans.strip()
        if util.is_equiv(extract_ans, answer):
            return True
        else:
            return False
    else:
        temp = {'question': doc, 'output': completion, 'answer': answer}
        invalid_outputs.append(temp)
        return False


def batch_data(data_list, batch_size=1):
    n = len(data_list) // batch_size
    batch_data = []
    for i in range(n-1):
        start = i * batch_size
        end = (i+1) * batch_size
        batch_data.append(data_list[start:end])
    last_start = (n-1) * batch_size
    batch_data.append(data_list[last_start:])
    return batch_data


# -------------------------
# Few-shot exemplars (k=5)
# -------------------------
FEW_SHOT_EXAMPLES = [
    {
        "q": "Compute 12+5.",
        "a": "17"
    },
    {
        "q": "What is 7*8?",
        "a": "56"
    },
    {
        "q": "If x=2, compute x^3.",
        "a": "8"
    },
    {
        "q": "Solve: 20/4.",
        "a": "5"
    },
    {
        "q": "Compute 15-6.",
        "a": "9"
    },
]


def build_zero_shot_prompt(instruction):
    return (
        "Below is an instruction that describes a task. "
        "Write a response that appropriately completes the request.\n\n"
        f"### Instruction:\n{instruction}\n\n### Response: Let's think step by step."
    )


def build_few_shot_prompt(instruction):
    prefix = "You are a helpful problem-solving assistant.\nLet's see some examples:\n\n"
    examples = ""
    for ex in FEW_SHOT_EXAMPLES:
        examples += f"Q: {ex['q']}\nA: {ex['a']}\n\n"

    suffix = (
        "Now solve the following problem.\n\n"
        f"Q: {instruction}\n"
        "A: Let's think step by step."
    )
    return prefix + examples + suffix


# -------------------------
# Main evaluation routine
# -------------------------
def test_hendrycks_math(
        model,
        data_path,
        mode="zero",
        start=0,
        end=MAX_INT,
        batch_size=1,
        tensor_parallel_size=1,
        random_sample=None
):
    hendrycks_math_ins = []
    hendrycks_math_answers = []

    # Load dataset
    with open(data_path, "r", encoding="utf8") as f:
        for idx, item in enumerate(jsonlines.Reader(f)):
            inst = item["instruction"]
            sol = item["output"]
            ans = remove_boxed(util.last_boxed_only_string(sol))

            if mode == "zero":
                prompt = build_zero_shot_prompt(inst)
            else:
                prompt = build_few_shot_prompt(inst)

            hendrycks_math_ins.append(prompt)
            hendrycks_math_answers.append(ans)

    total = len(hendrycks_math_ins)
    print(f"Total dataset size = {total}")

    # Subset sampling
    if random_sample is not None:
        idxs = random.sample(range(total), random_sample)
        hendrycks_math_ins = [hendrycks_math_ins[i] for i in idxs]
        hendrycks_math_answers = [hendrycks_math_answers[i] for i in idxs]
        print(f"Random sampled {random_sample} items.")
    else:
        hendrycks_math_ins = hendrycks_math_ins[start:end]
        hendrycks_math_answers = hendrycks_math_answers[start:end]

    print(f"Evaluation size = {len(hendrycks_math_ins)}")

    batch_ins = batch_data(hendrycks_math_ins, batch_size=batch_size)

    stop_tokens = [
        "Question:", "Question", "USER:", "USER",
        "ASSISTANT:", "ASSISTANT", "Instruction:",
        "Instruction", "Response:", "Response"
    ]
    sampling_params = SamplingParams(
        temperature=0,
        top_p=1,
        max_tokens=2048,
        stop=stop_tokens
    )
    print("Sampling params:", sampling_params)

    llm = LLM(model=model, tensor_parallel_size=tensor_parallel_size)

    res_completions = []
    for prompt in batch_ins:
        if not isinstance(prompt, list):
            prompt = [prompt]
        outs = llm.generate(prompt, sampling_params)
        for out in outs:
            res_completions.append(out.outputs[0].text)

    # Evaluate correctness
    results = []
    for prompt, comp, gt in zip(hendrycks_math_ins, res_completions, hendrycks_math_answers):
        ok = process_results(prompt, comp, gt)
        results.append(ok)

    acc = sum(results) / len(results)
    print(f"[Mode: {mode}] accuracy = {acc}")
    print("Invalid outputs count:", len(invalid_outputs))
    return acc


# -------------------------
# Argparse
# -------------------------
def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--model", type=str, required=True)
    parser.add_argument("--data_file", type=str, required=True)
    parser.add_argument("--mode", type=str, default="zero", choices=["zero", "few"])
    parser.add_argument("--start", type=int, default=0)
    parser.add_argument("--end", type=int, default=MAX_INT)
    parser.add_argument("--batch_size", type=int, default=400)
    parser.add_argument("--tensor_parallel_size", type=int, default=8)
    parser.add_argument("--random_sample", type=int, default=None)
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    test_hendrycks_math(
        model=args.model,
        data_path=args.data_file,
        mode=args.mode,
        start=args.start,
        end=args.end,
        batch_size=args.batch_size,
        tensor_parallel_size=args.tensor_parallel_size,
        random_sample=args.random_sample
    )
