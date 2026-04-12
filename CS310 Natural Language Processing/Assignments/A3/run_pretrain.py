# Copyright (c) Sebastian Raschka under Apache License 2.0 (see LICENSE.txt).
# Source for "Build a Large Language Model From Scratch"
#   - https://www.manning.com/books/build-a-large-language-model-from-scratch
# Code: https://github.com/rasbt/LLMs-from-scratch

"""
Script for pretraining a small GPT-2 124M parameter model
on Chinese Wikipedia text data.

Before running this script, make sure you:
1. Extracted and preprocessed the text data
2. Trained a BPE tokenizer on the text data
"""

import argparse
import os
import time
from pathlib import Path

from tokenizers import Tokenizer
import torch
from utils import (
    GPTModel,
    calc_loss_batch,
    create_dataloader_v1,
    evaluate_model,
    plot_losses,
    read_data_from_path,
)


def create_dataloaders(
    text_data, tokenizer, train_ratio, batch_size, max_length, stride, num_workers=0
):
    """Create training and validation dataloaders from text data."""
    split_idx = int(train_ratio * len(text_data))
    train_loader = create_dataloader_v1(
        text_data[:split_idx],
        tokenizer=tokenizer,
        batch_size=batch_size,
        max_length=max_length,
        stride=stride,
        drop_last=True,
        shuffle=True,
        num_workers=num_workers,
    )
    val_loader = create_dataloader_v1(
        text_data[split_idx:],
        tokenizer=tokenizer,
        batch_size=batch_size,
        max_length=max_length,
        stride=stride,
        drop_last=False,
        shuffle=False,
        num_workers=num_workers,
    )
    return train_loader, val_loader


def convert_time(seconds):
    """Convert seconds to hours, minutes, seconds."""
    hours, rem = divmod(seconds, 3600)
    minutes, seconds = divmod(rem, 60)
    return int(hours), int(minutes), int(seconds)


def train_model_simple(
    model,
    optimizer,
    device,
    n_epochs,
    eval_freq,
    eval_iter,
    output_dir,
    save_ckpt_freq,
    tokenizer,
    data_path,
    batch_size=1024,
    train_ratio=0.90,
):
    """
    Simple training loop for GPT model.
    
    Args:
        model: The GPT model to train
        optimizer: The optimizer
        device: Device to train on
        n_epochs: Number of epochs to train
        eval_freq: Evaluate every N steps
        eval_iter: Number of iterations for evaluation
        output_dir: Directory to save checkpoints
        save_ckpt_freq: Save checkpoint every N steps
        tokenizer: Tokenizer for encoding text
        data_path: Path to the training data file or directory
        batch_size: Batch size for training
        train_ratio: Ratio of data to use for training (rest for validation)
        
    Returns:
        Tuple of (train_losses, val_losses, track_tokens_seen)
    """
    ### START YOUR CODE ###
    # Initialize tracking variables
    train_losses, val_losses, track_tokens_seen = [], [], []
    tokens_seen = 0
    global_step = 0
    start_time = time.time()
    
    # TODO: Read the text data from data_path using read_data_from_path()
    
    # TODO: Add <|endoftext|> token at the end if not present
    
    # TODO: Create training and validation dataloaders using create_dataloaders()
    
    # TODO: Print useful information (total batches, vocab size, context length, data size)
    
    # TODO: Implement the training loop
    # For each epoch:
    #   - Set model to train mode
    #   - Iterate through train_loader batches
    #   - For each batch:
    #     * Zero gradients
    #     * Calculate loss using calc_loss_batch()
    #     * Backward pass
    #     * Optimizer step
    #     * Update tokens_seen and global_step
    #     * If global_step % eval_freq == 0: evaluate and record losses
    #     * If global_step % save_ckpt_freq == 0: save checkpoint
    
    # TODO: Handle KeyboardInterrupt to save interrupted checkpoint
    
    # TODO: Save final checkpoint
    
    ### END YOUR CODE ###
    
    return train_losses, val_losses, track_tokens_seen


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        description="GPT Model Training Configuration",
    )

    parser.add_argument(
        "--data_file", "--data",
        type=str,
        required=True,
        help="Path to the training data file or directory containing .txt files (e.g., data/wiki_zh_2019.txt or data/wiki_zh_2019/)",
    )
    parser.add_argument(
        "--output_dir",
        type=str,
        default="model_checkpoints",
        help="Directory where the model checkpoints will be saved",
    )
    parser.add_argument(
        "--n_epochs", type=int, default=1, help="Number of epochs to train the model"
    )
    parser.add_argument(
        "--tokenizer",
        type=str,
        required=True,
        help="Path to the tokenizer JSON file (e.g., tokenizer/wikizh_tokenizer_whitespace.json)",
    )
    parser.add_argument(
        "--eval_freq",
        type=int,
        default=100,
        help="Frequency of evaluations during training (in steps)",
    )
    parser.add_argument(
        "--save_ckpt_freq",
        type=int,
        default=100_000,
        help="Frequency of saving model checkpoints during training (in steps)",
    )
    parser.add_argument(
        "--lr", type=float, default=1e-4, help="Learning rate for the optimizer"
    )
    parser.add_argument(
        "--batch_size", type=int, default=4, help="Batch size for training"
    )
    parser.add_argument(
        "--train_ratio", type=float, default=0.90, help="Ratio of data for training (rest for validation)"
    )
    parser.add_argument(
        "--vocab_size", type=int, default=52000, help="Vocabulary size (should match tokenizer)"
    )
    parser.add_argument(
        "--debug",
        action="store_true",
        help="Uses a very small model for debugging purposes",
    )

    args = parser.parse_args()

    # Set model configuration
    if args.debug:
        GPT_CONFIG_124M = {
            "vocab_size": args.vocab_size,
            "context_length": 10,
            "emb_dim": 12,
            "n_heads": 2,
            "n_layers": 2,
            "drop_rate": 0.0,
            "qkv_bias": False,
        }
    else:
        GPT_CONFIG_124M = {
            "vocab_size": args.vocab_size,  # Should match tokenizer vocab size
            "context_length": 1024,  # Context length
            "emb_dim": 768,  # Embedding dimension
            "n_heads": 12,  # Number of attention heads
            "n_layers": 12,  # Number of layers
            "drop_rate": 0.1,  # Dropout rate
            "qkv_bias": False,  # Query-key-value bias
        }

    # Load tokenizer
    print(f"Loading tokenizer from: {args.tokenizer}")
    tokenizer = Tokenizer.from_file(args.tokenizer)
    
    # Verify vocab size matches
    actual_vocab_size = tokenizer.get_vocab_size()
    if actual_vocab_size != args.vocab_size:
        print(f"Warning: Tokenizer vocab size ({actual_vocab_size}) doesn't match --vocab_size ({args.vocab_size})")
        print(f"Updating model config to use vocab size: {actual_vocab_size}")
        GPT_CONFIG_124M["vocab_size"] = actual_vocab_size

    # Setup device
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")
    
    # Initialize model
    torch.manual_seed(123)
    model = GPTModel(GPT_CONFIG_124M)
    model.to(device)
    
    # Count parameters
    total_params = sum(p.numel() for p in model.parameters())
    print(f"Total model parameters: {total_params:,} ({total_params / 1e6:.2f}M)")
    
    # Setup optimizer
    optimizer = torch.optim.AdamW(model.parameters(), lr=args.lr, weight_decay=0.1)

    # Create output directory
    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)
    print(f"Checkpoints will be saved to: {output_dir.absolute()}")

    # Train model
    print("\nStarting training...")
    train_losses, val_losses, tokens_seen = train_model_simple(
        model=model,
        optimizer=optimizer,
        device=device,
        n_epochs=args.n_epochs,
        eval_freq=args.eval_freq,
        eval_iter=1,
        output_dir=output_dir,
        save_ckpt_freq=args.save_ckpt_freq,
        tokenizer=tokenizer,
        data_path=args.data_file,
        batch_size=args.batch_size,
        train_ratio=args.train_ratio,
    )

    ### START YOUR CODE ###
    # TODO: Plot losses if train_losses is not empty
    # Hint: Use torch.linspace to create epochs_tensor and call plot_losses()
    
    # TODO: Save the final model to output_dir / "model_final.pth"
    
    ### END YOUR CODE ###
    
    # Print GPU memory usage if CUDA is available
    if torch.cuda.is_available():
        print(f"Maximum GPU memory allocated: {torch.cuda.max_memory_allocated() / 1e9:.2f} GB")
    
    print("Training completed!")
