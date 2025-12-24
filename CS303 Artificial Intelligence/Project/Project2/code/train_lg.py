import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import warnings

from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score

from preprocess import DataPreprocessor

warnings.filterwarnings("ignore")


def run_logistic_regression(X_tr, y_tr, X_va, y_va, X_te, config):
    print("Training Logistic Regression Model...")
    print("\nLogistic Regression Configuration:")

    for k in config:
        print(f"  {k}: {config[k]}")

    clf = LogisticRegression(**config)
    clf.fit(X_tr, y_tr)

    print("\nTraining completed!")
    print(f"Number of iterations: {clf.n_iter_[0]}")

    val_pred = clf.predict(X_va)

    acc = accuracy_score(y_va, val_pred)
    prec = precision_score(y_va, val_pred)
    rec = recall_score(y_va, val_pred)
    f1 = f1_score(y_va, val_pred)

    print("Performance:")
    print(f"Accuracy:  {acc:.4f}")
    print(f"Precision: {prec:.4f}")
    print(f"Recall:    {rec:.4f}")
    print(f"F1 Score:  {f1:.4f}")

    coef_df = pd.DataFrame({
        "feature": X_tr.columns,
        "coe": clf.coef_[0]
    })

    coef_df["abs_coe"] = coef_df["coe"].abs()
    coef_df = coef_df.sort_values(by="abs_coe", ascending=False)

    print("Top 10 Most Important Features:")
    print(coef_df.head(10).to_string(index=False))

    test_pred = clf.predict(X_te)
    return test_pred


def main():
    print("Loading preprocessed data...")

    processor = DataPreprocessor(data_dir='..')
    X_all, y_all, X_test = processor.preprocess(
        feature_engineering=False,
        remove_low_predictive=False,
        one_hot_encoding=True,
        scale=True
    )

    X_train, X_val, y_train, y_val = train_test_split(
        X_all,
        y_all,
        test_size=0.2,
        stratify=y_all,
        random_state=42
    )

    print("\nData shapes:")
    print(f"  X_train: {X_train.shape}")
    print(f"  X_val:   {X_val.shape}")
    print(f"  X_test:  {X_test.shape}")

    lr_params = {
        "C": 0.1,
        "solver": "lbfgs",
        "max_iter": 1000,
        "random_state": 42,
        "verbose": 1,
        "n_jobs": -1,
        "class_weight": "balanced"
    }

    predictions = run_logistic_regression(
        X_train, y_train, X_val, y_val, X_test, lr_params
    )

    with open("../testlabel_lg.txt", "w") as fout:
        fout.write("\n".join(map(str, predictions)))
    print("Predictions saved as 'testlabel_lg.txt'")

    C_values = np.logspace(-3, 2, 8)
    acc_list, prec_list, rec_list, f1_list = [], [], [], []

    for C in C_values:
        clf = LogisticRegression(
            C=C,
            solver='lbfgs',
            max_iter=1000,
            class_weight='balanced',
            random_state=42,
            n_jobs=-1
        )
        clf.fit(X_train, y_train)
        preds = clf.predict(X_val)

        acc_list.append(accuracy_score(y_val, preds))
        prec_list.append(precision_score(y_val, preds, zero_division=0))
        rec_list.append(recall_score(y_val, preds, zero_division=0))
        f1_list.append(f1_score(y_val, preds, zero_division=0))

    # ===== Plot =====
    plt.figure(figsize=(7, 4))
    x = np.log10(C_values)

    plt.plot(x, acc_list, marker='o', label='Accuracy')
    plt.plot(x, prec_list, marker='s', label='Precision')
    plt.plot(x, rec_list, marker='^', label='Recall')
    plt.plot(x, f1_list, marker='d', label='F1 Score')

    plt.xlabel('log10(C)')
    plt.ylabel('Score')
    plt.title('Logistic Regression Performance vs Regularization')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.4)
    plt.tight_layout()
    plt.savefig('../lg.png')


if __name__ == "__main__":
    main()
