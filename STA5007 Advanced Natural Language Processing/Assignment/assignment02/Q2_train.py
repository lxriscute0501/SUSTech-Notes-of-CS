# nlp_training_script.py
"""
Goal
-----
Convert the decoding script into a minimal LoRA fine-tuning script.

1) Implement data preparation (prepare_dummy_data).
2) Implement the main training loop (train).
3) Modify main() to run the training on Qwen/Qwen2.5-0.5B-Instruct.

Notes
-----
- This script uses the model in a training configuration (forward pass with labels)
  instead of an inference/decoding configuration.
- We skip the LoRA injection part for simplicity, performing full (but inefficient)
  fine-tuning on a very small dummy dataset.
"""

from typing import Optional, List, Dict
import torch
import torch.nn as nn
from transformers import AutoTokenizer, AutoModelForCausalLM, DataCollatorForLanguageModeling
from torch.utils.data import Dataset, DataLoader
import torch.optim as optim
from modelscope import snapshot_download


# =======================================================
# ✨ 替换 DecodingSampler 为训练相关类和函数
# =======================================================

# -------------------------------------------------------
# 1. 数据集和准备函数
# -------------------------------------------------------
class SimpleInstructionDataset(Dataset):
    """虚拟指令数据集"""

    def __init__(self, tokenized_texts):
        self.tokenized_texts = tokenized_texts

    def __len__(self):
        return len(self.tokenized_texts["input_ids"])

    def __getitem__(self, i):
        return {
            "input_ids": self.tokenized_texts["input_ids"][i],
            "attention_mask": self.tokenized_texts["attention_mask"][i],
        }


def prepare_dummy_data(tokenizer, messages_list: List[List[Dict[str, str]]], max_length: int = 128):
    """
    将一系列对话消息列表转换为用于训练的 token ID。
    """

    # 将每条对话消息列表通过 chat template 转换为单条训练文本
    raw_texts = []
    for messages in messages_list:
        text_input = tokenizer.apply_chat_template(
            messages,
            tokenize=False,
            add_generation_prompt=False  # 训练时不需要 generation prompt
        )
        # Qwen 模型的模板通常以 <|im_end|> 结尾，需要确保它包含在训练文本中
        raw_texts.append(text_input)

    # Tokenize 并填充
    tokenized_texts = tokenizer(
        raw_texts,
        max_length=max_length,
        padding="max_length",
        truncation=True,
        return_tensors="pt"
    )

    print(f"Prepared {len(raw_texts)} training samples.")
    return SimpleInstructionDataset(tokenized_texts)


# -------------------------------------------------------
# 2. 核心训练循环
# -------------------------------------------------------
def train(model: nn.Module, tokenizer: AutoTokenizer, device: torch.device,
          messages_list: List[List[Dict[str, str]]],
          num_epochs: int = 3, batch_size: int = 2, lr: float = 1e-5):
    """
    核心训练循环（Full Fine-Tuning 模式）
    """
    print("\n--- Starting Full Fine-Tuning Training ---")
    model.train()

    # 1. 数据加载
    dummy_dataset = prepare_dummy_data(tokenizer, messages_list)
    data_loader = DataLoader(
        dummy_dataset,
        batch_size=batch_size,
        shuffle=True,
        # 使用 DataCollatorForLanguageModeling 准备语言模型所需的 labels
        collate_fn=DataCollatorForLanguageModeling(tokenizer, mlm=False)
    )

    # 2. 优化器 (对所有参数进行优化，因为是 Full Fine-Tuning)
    optimizer = optim.AdamW(model.parameters(), lr=lr)

    # 3. 训练循环
    for epoch in range(num_epochs):
        total_loss = 0
        for step, batch in enumerate(data_loader):
            # 移动数据到设备
            input_ids = batch['input_ids'].to(device)
            attention_mask = batch['attention_mask'].to(device)
            labels = batch['labels'].to(device)

            optimizer.zero_grad()

            # 前向传播：模型返回一个包含 loss 的对象
            outputs = model(
                input_ids=input_ids,
                attention_mask=attention_mask,
                labels=labels  # 传入 labels 后，模型内部会计算交叉熵损失
            )

            loss = outputs.loss

            # 反向传播和参数更新
            loss.backward()
            optimizer.step()

            total_loss += loss.item()

            if (step + 1) % 1 == 0:
                print(f"Epoch {epoch + 1}/{num_epochs}, Step {step + 1}, Loss: {loss.item():.4f}")

        avg_loss = total_loss / len(data_loader)
        print(f"Epoch {epoch + 1} finished. Average Loss: {avg_loss:.4f}")

    print("--- Training Complete ---")
    model.eval()
    return model


# -------------------------------------------
# 3. main 函数修改
# -------------------------------------------
def main():
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    # 模型名称
    model_name = "Qwen/Qwen2.5-0.5B-Instruct"

    # ✨ 关键修改：使用 snapshot_download 获取本地模型目录路径
    print(f"Downloading/Locating model: {model_name}...")
    # ⚠️ 请确保您已经运行过 snapshot_download，如果第一次运行，它会下载模型。
    model_dir = snapshot_download(model_name)
    print(f"Model local path: {model_dir}")

    # 使用本地路径加载 Tokenizer 和 Model
    tokenizer = AutoTokenizer.from_pretrained(model_dir)
    if tokenizer.pad_token is None and tokenizer.eos_token is not None:
        tokenizer.pad_token = tokenizer.eos_token

    model = AutoModelForCausalLM.from_pretrained(
        model_dir,
        # ✨ 修正：使用 dtype 代替 torch_dtype
        dtype=torch.bfloat16 if torch.cuda.is_available() else torch.float32,
    ).to(device)

    if getattr(model.config, "pad_token_id", None) is None and tokenizer.pad_token_id is not None:
        model.config.pad_token_id = tokenizer.pad_token_id

    # 虚拟训练数据 (指令和响应)
    training_messages = [
        [
            {"role": "system", "content": "You are a helpful assistant."},
            {"role": "user", "content": "Write a short poem about the sea."},
            {"role": "assistant",
             "content": "The waves crash on the shore, the gulls cry high above, endless blue and endless love."},
        ],
        [
            {"role": "system", "content": "You are a helpful assistant."},
            {"role": "user", "content": "What is the capital of Japan?"},
            {"role": "assistant", "content": "The capital of Japan is Tokyo."},
        ]
    ]

    # ✨ 调用训练函数
    trained_model = train(
        model,
        tokenizer,
        device,
        training_messages,
        num_epochs=5,
        batch_size=1,  # batch size 设置为 1 以适应小数据量
        lr=1e-5
    )

    # 训练后推理测试
    print("\n=== INFERENCE TEST AFTER TRAINING ===")

    # 新的指令用于测试
    test_messages = [
        {"role": "system", "content": "You are a helpful assistant."},
        {"role": "user", "content": "What is the capital of China?"},
    ]

    # 将指令转换为输入 ID
    text_input = tokenizer.apply_chat_template(test_messages, tokenize=False, add_generation_prompt=True)
    inputs = tokenizer(text_input, return_tensors="pt").to(device)

    trained_model.eval()
    with torch.no_grad():
        output = trained_model.generate(
            **inputs,  # 传入 input_ids 和 attention_mask
            max_new_tokens=30,  # 限制新生成的 token 数量
            do_sample=False,  # 使用 Greedy Decoding 进行确定性测试
            eos_token_id=tokenizer.eos_token_id,
        )

    decoded_output = tokenizer.decode(output[0], skip_special_tokens=True)
    print(f"Prompt:\n{text_input}")
    print(f"\nGenerated Output:\n{decoded_output}")


if __name__ == "__main__":
    torch.manual_seed(1234)
    main()