import torch
from datasets import load_dataset
from trl import SFTConfig, SFTTrainer
from transformers import AutoTokenizer, AutoModelForCausalLM
from peft import LoraConfig

MODEL_NAME = "Qwen/Qwen2.5-0.5B"
DATA_PATH = "data/filtered_sft_data.json"
OUTPUT_DIR = "checkpoints/sft_lora_output"
DEVICE = "mps" if torch.backends.mps.is_available() else ("cuda" if torch.cuda.is_available() else "cpu")

def main():
    print(f"Loading dataset from {DATA_PATH}...")
    dataset = load_dataset("json", data_files=DATA_PATH, split="train")

    def map_to_common_format(example):
        return {
            "text": f"User: {example['instruction']}\nAssistant: {example['output']}"
        }

    dataset = dataset.map(map_to_common_format, remove_columns=dataset.column_names)

    print("Loading tokenizer and model...")
    tokenizer = AutoTokenizer.from_pretrained(MODEL_NAME)
    if tokenizer.pad_token is None:
        tokenizer.pad_token = tokenizer.eos_token

    # Load in FP16 to save memory (Half-precision)
    model = AutoModelForCausalLM.from_pretrained(
        MODEL_NAME,
        torch_dtype=torch.float16,
        device_map=DEVICE
    )

    # --- LoRA Config ---
    lora_config = LoraConfig(
        r=8,
        lora_alpha=16,
        target_modules=["q_proj", "v_proj"],
        lora_dropout=0.05,
        bias="none",
        task_type="CAUSAL_LM"
    )

    sft_config = SFTConfig(
        output_dir=OUTPUT_DIR,
        per_device_train_batch_size=2,
        gradient_accumulation_steps=8,
        learning_rate=2e-4,
        num_train_epochs=3,
        save_steps=100,
        logging_steps=10,
        fp16=True,
        gradient_checkpointing=True,
        dataset_text_field="text",
    )

    print("Starting SFT with LoRA...")
    trainer = SFTTrainer(
        model=model,
        train_dataset=dataset,
        args=sft_config,
        peft_config=lora_config,
    )

    trainer.train()
    trainer.save_model(OUTPUT_DIR)
    print(f"SFT weights saved to {OUTPUT_DIR}")

if __name__ == "__main__":
    main()