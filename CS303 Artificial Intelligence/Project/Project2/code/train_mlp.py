import numpy as np
import torch
from torch import nn
from torch.utils.data import Dataset, DataLoader
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
import matplotlib.pyplot as plt
from preprocess import DataPreprocessor


class TabularDataset(Dataset):
    def __init__(self, frame, labels, num_keys, cat_keys):
        self.num_tensor = torch.as_tensor(frame[num_keys].values, dtype=torch.float32)
        self.cat_tensor = torch.as_tensor(frame[cat_keys].values, dtype=torch.long)
        self.label_tensor = None if labels is None else torch.as_tensor(labels, dtype=torch.float32)

    def __len__(self):
        return self.num_tensor.size(0)

    def __getitem__(self, index):
        if self.label_tensor is None:
            return self.num_tensor[index], self.cat_tensor[index]
        return self.num_tensor[index], self.cat_tensor[index], self.label_tensor[index]


class MLP(nn.Module):
    def __init__(self, cat_sizes, num_dim, hidden_sizes=(8, 8), dropout=0.2):
        super().__init__()

        self.emb_layers = nn.ModuleList([
            nn.Embedding(size, min(50, int(round(size ** 0.5))))
            for size in cat_sizes])

        emb_dim = sum(e.embedding_dim for e in self.emb_layers)
        dim_in = emb_dim + num_dim

        seq = []
        for h in hidden_sizes:
            seq.extend([
                nn.Linear(dim_in, h),
                nn.ReLU(),
                nn.BatchNorm1d(h),
                nn.Dropout(dropout)
            ])
            dim_in = h

        seq.append(nn.Linear(dim_in, 1))
        self.network = nn.Sequential(*seq)

    def forward(self, num_x, cat_x):
        emb_out = [layer(cat_x[:, i]) for i, layer in enumerate(self.emb_layers)]
        feat = torch.cat(emb_out + [num_x], dim=1)
        return self.network(feat).view(-1)


def normalize_numeric(train_df, val_df, test_df, cols):
    mu = train_df[cols].mean()
    sigma = train_df[cols].std().replace(0, 1)

    for df in (train_df, val_df, test_df):
        df[cols] = (df[cols] - mu) / sigma

    return train_df, val_df, test_df


def run_train_epoch(model, loader, loss_fn, optim, device):
    model.train()
    acc_loss = 0.0

    for num_x, cat_x, y in loader:
        num_x, cat_x, y = num_x.to(device), cat_x.to(device), y.to(device)
        optim.zero_grad()
        out = model(num_x, cat_x)
        loss = loss_fn(out, y)
        loss.backward()
        optim.step()
        acc_loss += loss.item() * y.size(0)

    return acc_loss / len(loader.dataset)


def run_eval_epoch(model, loader, device, loss_fn=None):
    model.eval()
    preds, golds = [], []
    total_loss, total_n = 0.0, 0

    with torch.no_grad():
        for num_x, cat_x, y in loader:
            num_x, cat_x, y = num_x.to(device), cat_x.to(device), y.to(device)
            logits = model(num_x, cat_x)
            prob = torch.sigmoid(logits)

            if loss_fn is not None:
                batch_loss = loss_fn(logits, y)
                total_loss += batch_loss.item() * y.size(0)
                total_n += y.size(0)

            preds.extend((prob.cpu().numpy() >= 0.5).astype(int))
            golds.extend(y.cpu().numpy())

    val_loss = None if total_n == 0 else total_loss / total_n
    acc = accuracy_score(golds, preds)
    prec = precision_score(golds, preds, zero_division=0)
    rec = recall_score(golds, preds, zero_division=0)
    f1 = f1_score(golds, preds, zero_division=0)

    return val_loss, acc, prec, rec, f1, preds, golds


def run_test_inference(model, loader, device):
    model.eval()
    outputs = []

    with torch.no_grad():
        for num_x, cat_x in loader:
            num_x, cat_x = num_x.to(device), cat_x.to(device)
            logits = model(num_x, cat_x)
            prob = torch.sigmoid(logits)
            outputs.extend((prob.cpu().numpy() >= 0.5).astype(int))

    return outputs


def main():
    device = torch.device("mps" if torch.mps.is_available() else "cpu")
    print(f"Using device: {device}")

    processor = DataPreprocessor(data_dir='..')
    X, y, X_test = processor.preprocess(
        feature_engineering=False,
        remove_low_predictive=False,
        one_hot_encoding=False,
        scale=False,
        fit=True
    )

    cat_cols = processor.categorical_features
    num_cols = processor.numerical_features

    X_tr, X_va, y_tr, y_va = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)

    X_tr, X_va, X_te = normalize_numeric(X_tr.copy(), X_va.copy(), X_test.copy(), num_cols)

    cat_sizes = [len(processor.label_encoders[c].classes_) for c in cat_cols]

    train_ds = TabularDataset(X_tr, y_tr, num_cols, cat_cols)
    val_ds = TabularDataset(X_va, y_va, num_cols, cat_cols)
    test_ds = TabularDataset(X_te, None, num_cols, cat_cols)

    train_loader = DataLoader(train_ds, batch_size=256, shuffle=True)
    val_loader = DataLoader(val_ds, batch_size=256, shuffle=False)
    test_loader = DataLoader(test_ds, batch_size=256, shuffle=False)

    model = MLP(cat_sizes, len(num_cols), hidden_sizes=(64, 32), dropout=0.2).to(device)
    loss_fn = nn.BCEWithLogitsLoss()
    optimizer = torch.optim.AdamW(model.parameters(), lr=1e-3, weight_decay=1e-2)

    epochs = 20
    best_f1, best_state = 0.0, None
    val_loss_hist, val_acc_hist = [], []

    for ep in range(1, epochs + 1):
        tr_loss = run_train_epoch(model, train_loader, loss_fn, optimizer, device)
        val_loss, acc, prec, rec, f1, _, _ = run_eval_epoch(
            model, val_loader, device, loss_fn
        )

        val_loss_hist.append(val_loss)
        val_acc_hist.append(acc)

        print(f"Epoch {ep:02d} | loss {tr_loss:.4f} | val_loss {val_loss:.4f} | "
              f"acc {acc:.4f} | prec {prec:.4f} | rec {rec:.4f} | f1 {f1:.4f}")

        if f1 > best_f1:
            best_f1 = f1
            best_state = model.state_dict()

    if best_state is not None:
        model.load_state_dict(best_state)

    val_loss, acc, prec, rec, f1, preds, golds = run_eval_epoch(
        model, val_loader, device, loss_fn
    )

    print(f"Accuracy:  {acc:.4f}")
    print(f"Precision: {prec:.4f}")
    print(f"Recall:    {rec:.4f}")
    print(f"F1 Score:  {f1:.4f}")

    test_preds = run_test_inference(model, test_loader, device)
    with open("../testlabel_mlp.txt", "w") as f:
        for v in test_preds:
            f.write(f"{v}\n")
    print("Predictions saved as 'testlabel_mlp.txt'")

    x_axis = list(range(1, epochs + 1))
    plt.figure(figsize=(8, 4))
    plt.plot(x_axis, val_loss_hist, label="Val Loss")
    plt.plot(x_axis, val_acc_hist, label="Val Accuracy")
    plt.xlabel("Epoch")
    plt.ylabel("Metric")
    plt.title("Validation Loss and Accuracy")
    plt.legend()
    plt.tight_layout()
    plt.savefig("../mlp.png")

if __name__ == "__main__":
    main()
