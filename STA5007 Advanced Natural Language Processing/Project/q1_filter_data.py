import json
import random
from typing import List, Dict, Any


def load_mock_data(size: int, data_type: str) -> List[Dict[str, Any]]:
    mock_data = []
    low_quality_samples = [
        {"instruction": "What is gravity?", "output": "Gravity is..."},
        {"instruction": "Explain calculus.", "output": "Calculus..."},
        {"instruction": "What is quantum mechanics?", "output": "Quantum mechanics is..."}
    ]

    for i in range(size):
        if data_type == "sft":
            instruction_len = random.randint(60, 300)
            instruction = f"Please provide a detailed explanation of a complex problem regarding {random.choice(['Newtonian Laws', 'Market Supply and Demand', 'Neural Networks'])} and include three constraints. (Sample {i}) " * (
                        instruction_len // 60)

            sample = {
                "id": f"sft_{i}",
                "instruction": instruction,
                "output": f"This is a comprehensive answer to the problem, containing {instruction_len} characters.",
            }
            if i % 500 == 0:
                mock_data.append(random.choice(low_quality_samples))

        elif data_type == "preference":
            question_len = random.randint(70, 250)
            question = f"Based on my last two learning sessions, how can I understand {random.choice(['Fourier Transform', 'Economic Recession', 'Linear Algebra'])} in a more intuitive way? (Sample {i})" * (
                        question_len // 70)

            sample = {
                "id": f"pref_{i}",
                "question": question,
                "chosen": "Use visual analogies and everyday comparisons for the explanation.",
                "rejected": "Use complex mathematical formulas and high-order abstract definitions.",
            }

        mock_data.append(sample)
    return mock_data


RAW_SAMPLES_COUNT = 1500
SFT_TARGET_COUNT = 1000
PREF_TARGET_COUNT = 1000

raw_sft_data = load_mock_data(RAW_SAMPLES_COUNT, "sft")
raw_pref_data = load_mock_data(RAW_SAMPLES_COUNT, "preference")

print(f"--- Initial Data Volume ---")
print(f"Raw SFT Samples: {len(raw_sft_data)}")
print(f"Raw Preference Samples: {len(raw_pref_data)}\n")


def heuristic_filter(raw_data: List[Dict[str, Any]], data_type: str, min_length: int = 50) -> List[Dict[str, Any]]:
    filtered_data = []
    seen_texts = set()
    text_key = "instruction" if data_type == "sft" else "question"

    for sample in raw_data:
        text = sample.get(text_key, "")

        if len(text) < min_length:
            continue
        if text in seen_texts:
            continue

        seen_texts.add(text)
        filtered_data.append(sample)

    return filtered_data

filtered_sft_data = heuristic_filter(raw_sft_data, data_type="sft", min_length=50)
filtered_pref_data = heuristic_filter(raw_pref_data, data_type="preference", min_length=70)

print(f"--- Data Volume After Filtering ---")
print(f"Filtered SFT Samples: {len(filtered_sft_data)}")
print(f"Filtered Preference Samples: {len(filtered_pref_data)}\n")


def truncate_data(data: List[Dict[str, Any]], target: int) -> List[Dict[str, Any]]:
    if len(data) > target:
        return data[:target]
    return data

final_sft_data = truncate_data(filtered_sft_data, SFT_TARGET_COUNT)
final_pref_data = truncate_data(filtered_pref_data, PREF_TARGET_COUNT)

print(f"--- Final Output Results ---")
print(f"Final SFT Sample Count (1k kept): {len(final_sft_data)}")
print(f"Final Preference Sample Count (1k kept): {len(final_pref_data)}")

print("\n--- SFT Filtered Result Example (First 2) ---")
print(json.dumps(final_sft_data[:2], indent=2))

print("\n--- Preference Filtered Result Example (First 2) ---")
print(json.dumps(final_pref_data[:2], indent=2))

with open("data/filtered_sft_data.json", "w", encoding="utf-8") as f:
    json.dump(final_sft_data, f, ensure_ascii=False, indent=2)

with open("data/filtered_preference_data.json", "w", encoding="utf-8") as f:
    json.dump(final_pref_data, f, ensure_ascii=False, indent=2)

print("\n--- Files Saved Successfully ---")
print("Saved: filtered_sft_data.json")
print("Saved: filtered_preference_data.json")