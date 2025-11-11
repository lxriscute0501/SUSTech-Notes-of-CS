# lora_assignment.py
"""
Goal
-----
Implement a minimal LoRA training toolkit:
1) A LoRA-wrapped Linear layer (class LoRALinear)
   - Complete: __init__, reset_parameters, merge, forward
2) A function to inject LoRA modules into a model
   - inject_lora_modules(model, r, alpha, dropout)
3) A function to report model parameter statistics
   - count_model_params(model): returns (trainable, total, ratio)

What to do
----------
- Fill in all TODO sections (EXCEPT main()) with correct implementations.
- Keep the TODOs in English.
- Use `main()` to test your code with Qwen/Qwen2.5-0.5B.
- You may add helper utilities if needed, but DO NOT change the function/class signatures that contain TODOs.

Notes
-----
- LoRA basics (for a Linear layer):
  Given a base weight W (out_features x in_features), LoRA adds a low-rank
  update ΔW = (B @ A) * (alpha / r), where:
    A: (r x in_features), B: (out_features x r)
  Forward with LoRA (not merged):
    y = x @ W^T + x @ (ΔW)^T
  When merged, W <- W + ΔW, and the bypass path can be disabled.

- When you "merge()" you should add ΔW into the base weight and mark the module
  as merged so that the LoRA bypass no longer applies in forward().

- The parameter statistics function should count:
    - total parameters in the entire model
    - trainable parameters (requires_grad == True)
    - ratio = trainable / total
"""

# pip install torch transformers accelerate datasets
import math
import torch
import torch.nn as nn
from typing import Tuple
from transformers import AutoModelForCausalLM, AutoTokenizer


# ---------------------------
# 1) LoRA Linear (fill the TODOs)
# ---------------------------
class LoRALinear(nn.Module):
    """
    Minimal LoRA: add a low-rank (A: in->r, B: r->out) side path
    on top of an existing nn.Linear.

    Forward (not merged):
        out = x @ W^T (+ bias) + ( x -> A -> B ) * (alpha / r)

    Forward (merged):
        out = x @ (W + ΔW)^T (+ bias), bypass disabled.

    TODOs you must complete:
      - __init__: store shapes, create lora_A / lora_B, scaling, dropout, and
                  keep references to base weight/bias. Track merged-state flag.
      - reset_parameters: initialize A with Kaiming uniform, B to zeros.
      - merge: add ΔW back to base weight in-place and disable bypass thereafter.
      - forward: implement both the base forward and the LoRA bypass (when not merged).
    """

    def __init__(self, base_linear: nn.Linear, r: int = 8, alpha: int = 16, dropout: float = 0.0):
        super().__init__()
        # TODO: Validate base_linear is nn.Linear; set in/out features
        # TODO: Save r, alpha and compute scaling = alpha / r
        # TODO: Keep references to base_linear.weight and base_linear.bias (DO NOT deep copy)
        # TODO: Create lora_A (in_features -> r, bias=False) and lora_B (r -> out_features, bias=False)
        # TODO: Create dropout module (nn.Dropout if dropout>0 else nn.Identity)
        # TODO: Track merged-state flag (e.g., self.merged = False)
        raise NotImplementedError

    def reset_parameters(self):
        """
        TODO:
        - Initialize lora_A with Kaiming uniform (nn.init.kaiming_uniform_ with a=sqrt(5))
        - Initialize lora_B to all zeros so the initial ΔW is zero
        """
        raise NotImplementedError

    @torch.no_grad()
    def merge(self):
        """
        TODO:
        - If already merged, return immediately.
        - Compute ΔW = (lora_B.weight @ lora_A.weight) * scaling
          Shapes note (nn.Linear uses [out, in]):
             lora_A.weight: [r, in]
             lora_B.weight: [out, r]
             resulting ΔW:  [out, in]
        - Add ΔW in-place to the base weight.
        - (Optional but recommended) zero out lora_A/B weights after merging.
        - Set merged flag so forward() stops using the bypass.
        """
        raise NotImplementedError

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        """
        TODO:
        - Compute base output: F.linear(x, base_weight, base_bias)
        - If NOT merged and r>0:
            * apply dropout to x
            * compute LoRA bypass: lora_B(lora_A(x)) * scaling
            * add to the base output
        - Return the final tensor
        """
        raise NotImplementedError


# ----------------------------------------
# 2) Inject LoRA: replace target Linear layers with LoRALinear (fill the TODO)
# ----------------------------------------
TARGET_LINEAR_KEYWORDS = [
    # Common attention/MLP projection names (Qwen/LLaMA style)
    "q_proj", "k_proj", "v_proj", "o_proj",
    "gate_proj", "up_proj", "down_proj",
]

def inject_lora_modules(model: nn.Module, r: int = 8, alpha: int = 16, dropout: float = 0.0) -> int:
    """
    Replace specific nn.Linear submodules (whose qualified names end with any of
    TARGET_LINEAR_KEYWORDS) with LoRALinear wrappers.

    TODO:
    - Iterate over named_modules().
    - If a module name endswith any keyword in TARGET_LINEAR_KEYWORDS and the module is nn.Linear:
        * Get the parent module via model.get_submodule(parent_name)
        * Create a LoRALinear(module, r, alpha, dropout) and move it to the same device/dtype as the base weight
        * setattr(parent, child_attr_name, lora_module)
        * Count how many replacements were made
    - Return the number of replaced modules.
    """
    raise NotImplementedError


# -------------------------------------------------------
# 3) Model parameter statistics (fill the TODO)
# -------------------------------------------------------
def count_model_params(model: nn.Module) -> Tuple[int, int, float]:
    """
    Return (trainable_params, total_params, ratio)

    TODO:
    - total = sum of numel() for all parameters
    - trainable = sum of numel() for parameters with requires_grad == True
    - ratio = trainable / total if total > 0 else 0.0
    """
    raise NotImplementedError


# -------------------------------------------------------
# Helper: mark only LoRA params as trainable (already implemented for you)
# -------------------------------------------------------
def mark_only_lora_as_trainable(model: nn.Module) -> int:
    """
    Freeze all params by default, then unfreeze LoRA matrices (A and B).
    Returns the number of trainable parameters after marking.
    """
    for p in model.parameters():
        p.requires_grad = False

    trainable = 0
    for m in model.modules():
        if isinstance(m, LoRALinear):
            if hasattr(m, "lora_A") and hasattr(m.lora_A, "weight"):
                m.lora_A.weight.requires_grad = True
                trainable += m.lora_A.weight.numel()
            if hasattr(m, "lora_B") and hasattr(m.lora_B, "weight"):
                m.lora_B.weight.requires_grad = True
                trainable += m.lora_B.weight.numel()
    return trainable


# -------------------------------------------
# 4) A minimal smoke test (do not modify this section)
# -------------------------------------------
def main():
    device = "cuda" if torch.cuda.is_available() else "cpu"
    model_name = "Qwen/Qwen2.5-0.5B"  # base (non-instruct) for causal LM
    tokenizer = AutoTokenizer.from_pretrained(model_name, use_fast=True)
    if tokenizer.pad_token is None and tokenizer.eos_token is not None:
        tokenizer.pad_token = tokenizer.eos_token

    model = AutoModelForCausalLM.from_pretrained(
        model_name,
        torch_dtype=torch.bfloat16 if torch.cuda.is_available() else torch.float32,
    ).to(device)

    if getattr(model.config, "pad_token_id", None) is None and tokenizer.pad_token_id is not None:
        model.config.pad_token_id = tokenizer.pad_token_id

    # Try LoRA injection
    print("\n=== INJECT ===")
    try:
        replaced = inject_lora_modules(model, r=8, alpha=16, dropout=0.05)
        print(f"[LoRA] Replaced linear layers: {replaced}")
    except NotImplementedError as e:
        print(f"NotImplementedError in inject_lora_modules: {e}")

    # Mark LoRA params as trainable
    print("\n=== TRAINABLE MARK ===")
    _ = mark_only_lora_as_trainable(model)

    # Parameter stats
    print("\n=== PARAM STATS ===")
    try:
        trainable, total, ratio = count_model_params(model)
        print(f"[Params] trainable={trainable:,}, total={total:,}, ratio={ratio:.4%}")
    except NotImplementedError as e:
        print(f"NotImplementedError in count_model_params: {e}")

    # Forward smoke test (will raise until you implement LoRALinear)
    print("\n=== FORWARD SMOKE TEST ===")
    sample = tokenizer("Hello Tokyo in autumn!", return_tensors="pt").to(device)
    try:
        with torch.no_grad():
            _ = model(**sample)
        print("Forward pass succeeded.")
    except Exception as e:
        print(f"Forward encountered an exception (expected until you finish TODOs): {e}")

    # Attempt to merge (will raise until you implement merge)
    print("\n=== MERGE ===")
    try:
        merged = 0
        for m in model.modules():
            if isinstance(m, LoRALinear):
                m.merge()
                merged += 1
        print(f"[LoRA] merged modules: {merged}")
    except NotImplementedError as e:
        print(f"NotImplementedError in LoRALinear.merge: {e}")


if __name__ == "__main__":
    torch.manual_seed(1234)
    main()
