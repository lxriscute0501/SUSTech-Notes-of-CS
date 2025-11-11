# nlp_decoding_assignment_chat_template_with_generate.py
"""
Goal
-----
Implement several autoregressive decoding strategies:
1) Greedy decoding
2) Top-K sampling
3) Top-P (nucleus) sampling
4) Min-P sampling

You may also add temperature scaling and repetition penalty if desired.

What to do
----------
- Fill in all TODO sections (except generate()).
- Use `main()` to test your code with Qwen/Qwen2-0.5B-Instruct.
"""

from typing import Optional, List, Dict
import torch
from transformers import AutoTokenizer, AutoModelForCausalLM


class DecodingSampler:
    """
    A unified decoding interface. Fill in the TODO sections.
    """

    def __init__(self, eos_token_id: Optional[int] = None, pad_token_id: Optional[int] = None):
        self.eos_token_id = eos_token_id
        self.pad_token_id = pad_token_id

    # -------------------------
    # Utility helpers
    # -------------------------
    def apply_temperature(self, logits: torch.Tensor, temperature: float) -> torch.Tensor:
        """Apply temperature scaling to logits."""
        # TODO: implement
        raise NotImplementedError


    def softmax_probs(self, logits: torch.Tensor) -> torch.Tensor:
        """Convert logits to probabilities."""
        # TODO: implement
        raise NotImplementedError

    def sample_from_probs(self, probs: torch.Tensor) -> int:
        """Sample a single token ID from a probability distribution."""
        # TODO: implement
        raise NotImplementedError

    # -------------------------
    # Decoding steps (single token)
    # -------------------------

    def topp_step(self, logits: torch.Tensor, p: float) -> int:
        """Top-P (nucleus) sampling: sample from smallest set with cumulative prob >= p."""
        # TODO: implement
        raise NotImplementedError

    def minp_step(self, logits: torch.Tensor, p_min: float) -> int:
        """Min-P sampling: keep tokens with prob >= p_min."""
        # TODO: implement
        raise NotImplementedError

    # -------------------------
    # Generation loop
    # -------------------------
    @torch.no_grad()
    def generate(
        self,
        model,
        tokenizer,
        messages: List[Dict[str, str]],
        max_new_tokens: int = 64,
        strategy: str = "greedy",
        temperature: float = 1.0,
        top_k: int = 50,
        top_p: float = 0.9,
        min_p: float = 1e-4,
        repetition_penalty: float = 1.0,
        stop_on_eos: bool = True,
        device: Optional[torch.device] = None
    ) -> str:
        """
        Full autoregressive generation loop.
        """
        if device is None:
            device = next(model.parameters()).device

        # 1. Apply chat template to format the conversation
        text_input = tokenizer.apply_chat_template(
            messages,
            tokenize=False,
            add_generation_prompt=True
        )

        # 2. Tokenize input and move to device
        enc = tokenizer(text_input, return_tensors="pt").to(device)
        input_ids = enc["input_ids"]
        generated = input_ids.clone()

        # 3. Autoregressive decoding
        for _ in range(max_new_tokens):
            outputs = model(input_ids=generated)
            logits = outputs.logits[:, -1, :]  # last token logits

            # Apply optional temperature
            logits = self.apply_temperature(logits, temperature)

            # Choose next token
            if strategy == "topp":
                next_id = self.topp_step(logits.squeeze(0), top_p)
            elif strategy == "minp":
                next_id = self.minp_step(logits.squeeze(0), min_p)
            else:
                raise ValueError(f"Unknown strategy: {strategy}")

            # Append token
            next_id_tensor = torch.tensor([[next_id]], device=device)
            generated = torch.cat([generated, next_id_tensor], dim=-1)

            # Stop if EOS token is reached
            if stop_on_eos and self.eos_token_id is not None and next_id == self.eos_token_id:
                break

        # 4. Decode only the continuation
        output_text = tokenizer.decode(generated[0, input_ids.shape[-1]:], skip_special_tokens=True)
        return output_text.strip()


def main():
    """
    Example usage with Qwen/Qwen2.5-0.5B-Instruct.
    """
    model_name = "Qwen/Qwen2.5-0.5B-Instruct"
    tokenizer = AutoTokenizer.from_pretrained(model_name)
    model = AutoModelForCausalLM.from_pretrained(model_name)
    sampler = DecodingSampler(tokenizer.eos_token_id, tokenizer.pad_token_id)

    messages = [
        {"role": "system", "content": "You are a helpful assistant."},
        {"role": "user", "content": "Write a two-sentence travel tip for visiting Tokyo in autumn."},
    ]

    tests = [
        ("topp", {"strategy": "topp", "top_p": 0.9, "temperature": 0.8}),
        ("minp", {"strategy": "minp", "min_p": 1e-3, "temperature": 0.8}),
    ]

    for name, kwargs in tests:
        print(f"\n=== {name.upper()} ===")
        try:
            print(sampler.generate(model, tokenizer, messages, max_new_tokens=80, **kwargs))
        except NotImplementedError as e:
            print(f"NotImplementedError: {e}")


if __name__ == "__main__":
    torch.manual_seed(1234)
    main()