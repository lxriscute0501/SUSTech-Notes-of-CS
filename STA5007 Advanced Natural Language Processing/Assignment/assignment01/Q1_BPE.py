import json
from subword_nmt import learn_bpe, apply_bpe

def load_jsonl_texts(filename):
    data = []
    with open(filename, 'r', encoding='utf-8') as f:
        for line in f:
            if not line.strip():
                continue
            obj = json.loads(line.strip())
            conversations = obj.get("conversations", [])
            chatml_segments = []

            for conv in conversations:
                role = conv.get("from", "")
                text = conv.get("value", "").strip()
                if not text:
                    continue
                if role == "human":
                    chatml_segments.append(f"<|user|>\n{text}")
                elif role == "gpt":
                    chatml_segments.append(f"<|assistant|>\n{text}")
                else:
                    chatml_segments.append(f"<|{role}|>\n{text}")

            chatml_text = "\n".join(chatml_segments) + "\n<|end|>"
            data.append(chatml_text)
    return data

def prepare_training_file(data, filename="chatml_data.txt"):
    with open(filename, "w", encoding="utf-8") as f:
        for text in data:
            clean_text = text.replace("\n", " ")
            f.write(clean_text + "\n")
    print(filename)

def train_bpe(input_file="chatml_data.txt", codes_file="bpe.codes", vocab_size=10000):
    with open(input_file, encoding="utf-8") as input_data, open(codes_file, "w", encoding="utf-8") as output_codes:
        learn_bpe.learn_bpe(input_data, output_codes, num_symbols=vocab_size)
    print(vocab_size, codes_file)

def apply_bpe_to_text(codes_file="bpe.codes", test_sentence="hello world"):
    with open(codes_file, encoding="utf-8") as codes:
        bpe = apply_bpe.BPE(codes)
        tokenized = bpe.process_line(test_sentence)
        print("Original:", test_sentence)
        print("After BPE:", tokenized)


if __name__ == "__main__":
    data = load_jsonl_texts("industry_instruction_semantic_cluster_dedup_%E7%A7%91%E6%8A%80_%E7%A7%91%E5%AD%A6%E7%A0%94%E7%A9%B6_valid_train.jsonl")
    prepare_training_file(data)
    train_bpe(vocab_size=10000)
    apply_bpe_to_text()