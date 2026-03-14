import torch
import os
from datasets import load_dataset
from transformers import AutoTokenizer, AutoModelForCausalLM
from trl import DPOConfig, DPOTrainer
from peft import PeftModel

BASE_MODEL = "Qwen/Qwen2.5-0.5B"
SFT_LORA_PATH = "checkpoints/sft_lora_output"
DPO_OUTPUT_DIR = "checkpoints/dpo_lora_output"
PREF_DATA_PATH = "data/filtered_preference_data.json"
DEVICE = "mps" if torch.backends.mps.is_available() else ("cuda" if torch.cuda.is_available() else "cpu")


def main():
    print("Loading tokenizer...")
    tokenizer = AutoTokenizer.from_pretrained(BASE_MODEL)
    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token

    print(f"Loading preference dataset from {PREF_DATA_PATH}...")
    dataset = load_dataset("json", data_files=PREF_DATA_PATH, split="train")

    def process_dpo_data(example):
        return {
            "prompt": f"User: {example['question']}\nAssistant: ",
            "chosen": example["chosen"],
            "rejected": example["rejected"],
        }

    dataset = dataset.map(process_dpo_data, remove_columns=dataset.column_names)

    print("Loading base model with SFT adapters...")
    model = AutoModelForCausalLM.from_pretrained(
        BASE_MODEL,
        torch_dtype=torch.float16,
        device_map=DEVICE
    )

    model = PeftModel.from_pretrained(model, SFT_LORA_PATH, is_trainable=True)

    if hasattr(model, "enable_input_require_grads"):
        model.enable_input_require_grads()
    else:
        model.get_input_embeddings().register_forward_hook(lambda module, input, output: output.requires_grad_(True))

    dpo_config = DPOConfig(
        output_dir=DPO_OUTPUT_DIR,
        per_device_train_batch_size=1,
        gradient_accumulation_steps=16,
        num_train_epochs=3,
        learning_rate=5e-5,
        beta=0.1,
        max_length=512,
        max_prompt_length=256,
        fp16=True,
        gradient_checkpointing=True,
        dataloader_pin_memory=False,
        logging_steps=10,
        remove_unused_columns=False,
    )

    print("Initializing DPOTrainer...")
    trainer = DPOTrainer(
        model=model,
        ref_model=None,
        args=dpo_config,
        train_dataset=dataset,
    )

    print("Starting DPO training...")
    trainer.train()

    print(f"Saving DPO model to {DPO_OUTPUT_DIR}...")
    trainer.save_model(DPO_OUTPUT_DIR)
    print("DPO training finished.")


if __name__ == "__main__":
    main()