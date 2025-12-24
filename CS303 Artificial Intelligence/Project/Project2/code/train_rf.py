import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score
from sklearn.ensemble import RandomForestClassifier
from preprocess import DataPreprocessor
import warnings

warnings.filterwarnings('ignore')

def train_random_forest(X_train, y_train, X_val, y_val, X_test, params):
    print("Training Random Forest Model...")
    print("\nRandom Forest Configuration:")
    for key, value in params.items():
        print(f"  {key}: {value}")

    model = RandomForestClassifier(
        n_estimators=params['n_estimators'],
        max_depth=params['max_depth'],
        min_samples_split=params['min_samples_split'],
        min_samples_leaf=params['min_samples_leaf'],
        max_features=params['max_features'],
        class_weight=params['class_weight'],
        random_state=42,
        n_jobs=-1
    )

    model.fit(X_train, y_train)
    y_val_pred = model.predict(X_val)
    val_accuracy = accuracy_score(y_val, y_val_pred)
    val_precision = precision_score(y_val, y_val_pred)
    val_recall = recall_score(y_val, y_val_pred)
    val_f1 = f1_score(y_val, y_val_pred)

    print("Validation Set Performance:")
    print(f"Accuracy:  {val_accuracy:.4f}")
    print(f"Precision: {val_precision:.4f}")
    print(f"Recall:    {val_recall:.4f}")
    print(f"F1 Score:  {val_f1:.4f}")

    print("\n" + "=" * 60)
    print("Top 10 Most Important Features:")
    print("=" * 60)
    importance = model.feature_importances_
    importance_df = pd.DataFrame({
        'feature': X_train.columns,
        'importance': importance
    }).sort_values('importance', ascending=False).head(10)
    print(importance_df.to_string(index=False))

    y_test_pred = model.predict(X_test)

    return y_test_pred


def main():
    print("Loading preprocessed data...")
    preprocessor = DataPreprocessor(data_dir='..')
    X_train, y_train, X_test = preprocessor.preprocess(
        feature_engineering=True,
        remove_low_predictive=False,
        one_hot_encoding=True,
        scale=False
    )

    X_train, X_val, y_train, y_val = train_test_split(
        X_train, y_train,
        test_size=0.2,
        random_state=42,
        stratify=y_train
    )

    print(f"\nData shapes:")
    print(f"  X_train: {X_train.shape}")
    print(f"  X_val:   {X_val.shape}")
    print(f"  X_test:  {X_test.shape}")

    y_train = np.array(y_train)
    y_val = np.array(y_val)

    class_weight = {0: 1.0, 1: sum(y_train == 0) / sum(y_train == 1)}

    params = {
        'n_estimators': 300,
        'max_depth': None,
        'min_samples_split': 2,
        'min_samples_leaf': 1,
        'max_features': 'sqrt',
        'class_weight': class_weight
    }

    y_pred = train_random_forest(
        X_train, y_train,
        X_val, y_val,
        X_test, params
    )

    with open('../testlabel_rf.txt', 'w') as f:
        for label in y_pred:
            f.write(f"{label}\n")
    print("Predictions saved as 'testlabel_rf.txt'")

    n_list = [10, 20, 50, 100, 150]
    acc_scores = []

    for n in n_list:
        rf = RandomForestClassifier(
            n_estimators=n,
            random_state=42,
            n_jobs=-1
        )
        rf.fit(X_train, y_train)
        preds = rf.predict(X_val)
        acc_scores.append(accuracy_score(y_val, preds))

    plt.figure(figsize=(6, 4))
    plt.plot(n_list, acc_scores, marker='o')
    plt.xlabel("Number of Trees")
    plt.ylabel("Validation Accuracy")
    plt.title("Random Forest: Trees vs Accuracy")
    plt.tight_layout()
    plt.savefig("../rf.png")


if __name__ == '__main__':
    main()
