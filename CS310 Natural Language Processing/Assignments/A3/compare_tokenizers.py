from tokenizers import Tokenizer
from transformers import AutoTokenizer

# Load the tokenizer trained from scratch
tokenizer_from_scratch = Tokenizer.from_file("wikizh_tokenizer_whitespace.json")

# Original GPT-2 tokenizer from Hugging Face
tokenizer_gpt2_original = AutoTokenizer.from_pretrained("gpt2")

# Tokenize a sample text
text = "太阳照常升起。"
tokenids_from_scratch = tokenizer_from_scratch.encode(text).ids
tokenids_gpt2_original = tokenizer_gpt2_original.encode(text)

str_list1 = [tokenizer_from_scratch.decode([tid]) for tid in tokenids_from_scratch]
str_list2 = [tokenizer_gpt2_original.decode([tid]) for tid in tokenids_gpt2_original]

print("Trained wikizh_tokenizer:", tokenids_from_scratch, str_list1)
print("Original GPT-2 tokenizer:", tokenids_gpt2_original, str_list2)
