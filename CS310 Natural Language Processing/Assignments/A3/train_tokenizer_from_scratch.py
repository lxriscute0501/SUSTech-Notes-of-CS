import argparse

from tokenizers import Tokenizer, models, pre_tokenizers, trainers

parser = argparse.ArgumentParser(
    description="Train a BPE tokenizer on Chinese text data from scratch."
)
parser.add_argument(
    "--input", type=str, required=True, help="Path to the input Chinese text file."
)
parser.add_argument(
    "--vocab_size", type=int, default=52000, help="Vocabulary size."
)
parser.add_argument(
    "--pre_tokenizer", type=str, choices=["Whitespace", "ByteLevel"], default="Whitespace", help="Pre-tokenizer to use."
)
parser.add_argument(
    "--min_freq", type=int, default=2, help="Minimum frequency for a word to be included in the vocabulary."
)
parser.add_argument(
    "--output", type=str, default="wikizh_tokenizer_from_scratch.json", help="Path to save the trained tokenizer."
)
args = parser.parse_args()

# Initialize a BPE tokenizer
tokenizer = Tokenizer(models.BPE(unk_token="<|endoftext|>"))

# Pre-tokenize 
if args.pre_tokenizer == "Whitespace":
    tokenizer.pre_tokenizer = pre_tokenizers.Whitespace()
elif args.pre_tokenizer == "ByteLevel":
    tokenizer.pre_tokenizer = pre_tokenizers.ByteLevel()

# Train on your chinese text file
trainer = trainers.BpeTrainer(
    vocab_size=args.vocab_size, 
    special_tokens=["<|endoftext|>"],
    min_frequency=args.min_freq,
)
tokenizer.train([args.input], trainer)

# Save the tokenizer
tokenizer.save(args.output)
