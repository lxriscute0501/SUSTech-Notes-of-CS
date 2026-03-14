import torch
import requests
from transformers import AutoTokenizer, AutoModelForCausalLM
from peft import PeftModel

BASE_MODEL_PATH = "Qwen/Qwen2.5-0.5B"
SFT_LORA_PATH = "checkpoints/sft_lora_output"
DPO_LORA_PATH = "checkpoints/dpo_lora_output"
DEEPSEEK_API_KEY = "YOUR_DEEPSEEK_API_KEY"
DEEPSEEK_API_URL = "https://api.deepseek.com/v1/chat/completions"
DEVICE = "mps" if torch.backends.mps.is_available() else ("cuda" if torch.cuda.is_available() else "cpu")

test_samples = [
    {
        "instruction": "Explain the concept of 'Photosynthesis' for a junior high school student.",
        "reference": "Photosynthesis is the process by which plants use sunlight, water, and carbon dioxide to create oxygen and energy in the form of sugar."
    },
    {
        "instruction": "Calculate 15 times 12 and explain the steps.",
        "reference": "15 * 12 = 15 * (10 + 2) = 150 + 30 = 180."
    },
    {
        "instruction": "Describe Newton's First Law of Motion in simple terms.",
        "reference": "An object at rest stays at rest, and an object in motion stays in motion unless acted upon by an unbalanced force."
    },
    {
        "instruction": "How can I identify a 'Passive Voice' sentence in English?",
        "reference": "Look for a form of the verb 'to be' followed by a past participle (e.g., 'is eaten', 'was broken')."
    },
    {
        "instruction": "Why is environmental protection important? Give two reasons.",
        "reference": "1. To maintain ecological balance; 2. To ensure human health and sustainable development."
    }
]


def generate_response(model_path):
    print(f"Loading model for inference: {model_path}...")
    tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL_PATH)
    base_model = AutoModelForCausalLM.from_pretrained(
        BASE_MODEL_PATH,
        torch_dtype=torch.float16,
        device_map=DEVICE
    )

    model = PeftModel.from_pretrained(base_model, model_path).eval()

    responses = []
    for sample in test_samples:
        prompt = f"User: {sample['instruction']}\nAssistant: "
        inputs = tokenizer(prompt, return_tensors="pt").to(DEVICE)

        with torch.no_grad():
            outputs = model.generate(**inputs, max_new_tokens=150)

        full_text = tokenizer.decode(outputs[0], skip_special_tokens=True)
        responses.append(full_text.split("Assistant:")[-1].strip())

    del model, base_model
    if torch.cuda.is_available(): torch.cuda.empty_cache()
    return responses


def judge_with_deepseek(instruction, response, reference):
    prompt = f"""
    You are an expert education consultant. Please evaluate the student assistant's response based on the instruction and reference answer.

    Criteria: Accuracy, Clarity, and Pedagogical Suitability (0-10 scale).

    Instruction: {instruction}
    Reference Answer: {reference}
    Assistant's Response: {response}

    Please output only the score (e.g., 8.5/10) followed by a one-sentence justification.
    """

    headers = {
        "Authorization": f"Bearer {DEEPSEEK_API_KEY}",
        "Content-Type": "application/json"
    }
    payload = {
        "model": "deepseek-chat",
        "messages": [{"role": "user", "content": prompt}],
        "temperature": 0
    }

    try:
        res = requests.post(DEEPSEEK_API_URL, json=payload, headers=headers)
        return res.json()['choices'][0]['message']['content']
    except Exception as e:
        return f"Error during judging: {e}"


def run_evaluation():
    sft_responses = generate_response(SFT_LORA_PATH)
    dpo_responses = generate_response(DPO_LORA_PATH)

    print("\n" + "=" * 30)
    print("LLM-AS-A-JUDGE EVALUATION REPORT")
    print("=" * 30)

    for i in range(len(test_samples)):
        print(f"\n[Test Case {i + 1}]")
        print(f"Question: {test_samples[i]['instruction']}")

        print(f"\n--- SFT Model ---")
        print(f"Response: {sft_responses[i]}")
        sft_score = judge_with_deepseek(test_samples[i]['instruction'], sft_responses[i], test_samples[i]['reference'])
        print(f"Judge Feedback: {sft_score}")

        print(f"\n--- DPO Model ---")
        print(f"Response: {dpo_responses[i]}")
        dpo_score = judge_with_deepseek(test_samples[i]['instruction'], dpo_responses[i], test_samples[i]['reference'])
        print(f"Judge Feedback: {dpo_score}")
        print("-" * 20)


if __name__ == "__main__":
    run_evaluation()