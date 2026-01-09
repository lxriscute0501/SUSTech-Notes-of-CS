import random
import numpy as np

seed_data = [
    ("Explain the difference between supervised and unsupervised learning.",
     "Supervised learning uses labeled data, while unsupervised learning discovers hidden patterns in unlabeled data."),
    ("What is overfitting in machine learning?",
     "Overfitting happens when a model memorizes noise instead of learning patterns."),
    ("Define gradient descent.",
     "Gradient descent is an optimization algorithm that updates parameters by moving opposite the gradient."),
    ("What is a decision tree?",
     "A decision tree is a flowchart-like model that splits data based on feature values."),
    ("Explain regularization.",
     "Regularization adds penalties to prevent overfitting."),
    ("What is a neural network?",
     "A neural network consists of interconnected layers that learn hierarchical representations."),
    ("Define reinforcement learning.",
     "Reinforcement learning trains agents via rewards and punishments."),
    ("What is PCA?",
     "PCA reduces dimensionality by projecting data onto principal components."),
    ("Explain the bias–variance tradeoff.",
     "The tradeoff describes balancing model complexity and generalization."),
    ("What is a loss function?",
     "A loss function measures prediction error.")
]


def simulate_llm_candidates(prompt, k=3):
    templates = [
        f"{prompt} In simple terms, it means ...",
        f"This question refers to fundamental ML concepts. Basically, ...",
        f"A concise explanation is as follows: ..."
    ]
    return [random.choice(templates) for _ in range(k)]


def reward_function(prompt, response):
    length_score = min(len(response) / 200, 1.0)

    keywords = ["learning", "model", "data", "algorithm"]
    keyword_score = sum(kw in response.lower() for kw in keywords) / len(keywords)

    overlap = len(set(prompt.lower().split()) & set(response.lower().split()))
    pseudo_bleu = overlap / max(len(prompt.split()), 1)

    total = 0.5 * length_score + 0.3 * keyword_score + 0.2 * pseudo_bleu
    return total


def run_filtering(seed_data):
    for idx, (instruction, _) in enumerate(seed_data):
        print("=" * 80)
        print(f"Prompt {idx + 1}: {instruction}\n")

        candidates = simulate_llm_candidates(instruction, k=3)

        scores = []
        for i, resp in enumerate(candidates):
            score = reward_function(instruction, resp)
            scores.append(score)
            print(f"Candidate {i + 1}: {resp}")
            print(f"Score = {score:.4f}\n")

        best_idx = int(np.argmax(scores))
        print(f"Selected: Candidate {best_idx + 1}")
        print("\n")


if __name__ == "__main__":
    run_filtering(seed_data)
