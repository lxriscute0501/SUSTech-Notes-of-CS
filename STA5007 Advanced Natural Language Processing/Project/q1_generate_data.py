import json
import random
import time
from typing import List, Dict
import pandas as pd


class LLMClient:
    def generate(self, system_prompt: str, user_prompt: str, temperature: float = 0.7) -> str:
        return f"[Simulated Output] content based on: {user_prompt[:30]}..."

client = LLMClient()

SEED_TOPICS = [
    "Linear Algebra", "Macroeconomics", "Organic Chemistry",
    "European History", "Data Structures", "Quantum Mechanics",
    "Cognitive Psychology", "Marketing Principles"
]

EVOL_PROMPTS = [
    "Rewrite the following question to be more complex by adding multiple constraints.",
    "Rewrite the following question to require multi-step reasoning.",
    "Rewrite the following question to explicitly require a specific difficulty level (e.g., PhD level vs Undergraduate level).",
    "Concretize the following abstract concept into a real-world case study problem.",
    "Add a requirement to identify common misconceptions in the answer."
]


class WizardEducationGenerator:
    def __init__(self, client, target_count=2000):
        self.client = client
        self.target_count = target_count
        self.data = []

    def evolve_instruction(self, seed_topic: str) -> str:
        evolution_method = random.choice(EVOL_PROMPTS)

        prompt = f"""
        I want you to act as an educational content creator. 
        1. Start with this topic: "{seed_topic}".
        2. {evolution_method}
        3. Output ONLY the new, complex question/instruction.
        """
        return self.client.generate("You are an expert curriculum designer.", prompt)

    def generate_educational_response(self, instruction: str) -> str:
        prompt = f"""
        You are a personalized education copilot. 
        Instruction: {instruction}

        Provide a comprehensive, step-by-step educational response. 
        - If the difficulty is high, use technical depth.
        - If the difficulty is low, use analogies.
        - Break down the solution clearly.
        """
        return self.client.generate("You are a helpful tutor.", prompt)

    def run(self):
        print(f"--- Starting WizardLM SFT Generation ({self.target_count} samples) ---")
        while len(self.data) < self.target_count:
            topic = random.choice(SEED_TOPICS)
            instruction = self.evolve_instruction(topic)
            response = self.generate_educational_response(instruction)

            self.data.append({
                "id": f"sft_{len(self.data)}",
                "source": "wizard_evol_edu",
                "instruction": instruction,
                "output": response
            })

            if len(self.data) % 100 == 0:
                print(f"Generated {len(self.data)} SFT samples...")
        return self.data

STUDENT_PERSONAS = [
    {"type": "Struggling Freshman", "preference": "Needs analogies, simple language, step-by-step breakdown.",
     "level": "Easy"},
    {"type": "Advanced Researcher", "preference": "Prefers concise, rigorous proofs, formal notation.",
     "level": "Hard"},
    {"type": "Visual Learner", "preference": "Asks for diagrams (descriptions), spatial examples.", "level": "Medium"},
    {"type": "Exam Crammer", "preference": "Needs cheat-sheet style summaries, key formulas only.", "level": "Medium"}
]


class FSPOPreferenceGenerator:
    def __init__(self, client, target_count=2000):
        self.client = client
        self.target_count = target_count
        self.data = []

    def generate_fspo_sample(self):
        persona = random.choice(STUDENT_PERSONAS)
        topic = random.choice(SEED_TOPICS)

        history_prompt = f"User Persona: {persona['type']}. Topic: {topic}. Create a short conversation history (2 turns) where the user asks a question and the AI replies in a way that the user LIKES ({persona['preference']})."
        context_history = self.client.generate("Simulate a student-AI chat history.", history_prompt)

        query_prompt = f"Based on the previous history, what is the next logical question this student ({persona['type']}) would ask about {topic}?"
        user_query = self.client.generate("Simulate a student query.", query_prompt)

        chosen_prompt = f"""
        Context: {context_history}
        User Query: {user_query}
        Persona Constraint: The user requires {persona['preference']}.

        Write a response that PERFECTLY fits this constraint.
        """
        chosen_response = self.client.generate("You are an adaptive tutor.", chosen_prompt)

        rejected_prompt = f"""
        Context: {context_history}
        User Query: {user_query}
        Persona Constraint: The user requires {persona['preference']}.

        Write a response that is factually correct but WRONG for this specific user (e.g., if they want simple, give them complex; if they want detail, give them brief).
        """
        rejected_response = self.client.generate("You are a rigid textbook.", rejected_prompt)

        return {
            "id": f"pref_{len(self.data)}",
            "source": "fspo_edu",
            "system_prompt": f"You are an adaptive educational agent. Adjust to the user's level: {persona['level']}",
            "history": context_history,
            "question": user_query,
            "chosen": chosen_response,
            "rejected": rejected_response
        }

    def run(self):
        print(f"--- Starting FSPO Preference Generation ({self.target_count} samples) ---")
        while len(self.data) < self.target_count:
            sample = self.generate_fspo_sample()
            self.data.append(sample)

            if len(self.data) % 100 == 0:
                print(f"Generated {len(self.data)} Preference samples...")

        return self.data

if __name__ == "__main__":
    sft_gen = WizardEducationGenerator(client, target_count=2000)
    sft_dataset = sft_gen.run()

    pref_gen = FSPOPreferenceGenerator(client, target_count=2000)
    pref_dataset = pref_gen.run()

    with open("data/sft_data.jsonl", "w") as f:
        for entry in sft_dataset:
            f.write(json.dumps(entry) + "\n")

    with open("data/preference_data.jsonl", "w") as f:
        for entry in pref_dataset:
            f.write(json.dumps(entry) + "\n")

    print("Data synthesis complete. Files saved.")