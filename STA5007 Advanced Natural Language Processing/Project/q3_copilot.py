import torch

class EduAgent:
    def __init__(self, role, system_prompt):
        self.role = role
        self.system_prompt = system_prompt

    def call(self, user_input, context=""):
        prompt = f"{self.system_prompt}\nContext: {context}\nUser: {user_input}\nAssistant:"
        return f"[{self.role} Response] to '{user_input[:20]}...'"


SUPERVISOR_PROMPT = """You are the Coordinator of an Educational Copilot. 
Your goal is to manage the student's learning path. 
1. Identify if the student wants to 'Learn', 'Take a Quiz', or 'Get Feedback'.
2. Route the request to the appropriate agent.
3. Keep track of the student's current difficulty level (Beginner, Intermediate, Advanced)."""

TUTOR_PROMPT = """You are a Personalized Tutor. 
Explain complex concepts using the required difficulty level. 
- For Beginners: Use analogies and simple language.
- For Advanced: Use rigorous definitions and technical depth."""

EXAMINER_PROMPT = """You are the Assessment Specialist. 
Generate questions that test both conceptual understanding and application. 
Evaluate student answers and provide the correct reasoning."""


class MultiAgentCopilot:
    def __init__(self):
        self.supervisor = EduAgent("Supervisor", SUPERVISOR_PROMPT)
        self.tutor = EduAgent("Tutor", TUTOR_PROMPT)
        self.examiner = EduAgent("Examiner", EXAMINER_PROMPT)
        self.difficulty = "Beginner"

    def chat(self, user_message):
        if "test" in user_message.lower() or "quiz" in user_message.lower():
            response = self.examiner.call(user_message, f"Level: {self.difficulty}")
        else:
            response = self.tutor.call(user_message, f"Level: {self.difficulty}")
        return response

copilot = MultiAgentCopilot()
print(copilot.chat("Can you explain Quantum Entanglement for a beginner?"))