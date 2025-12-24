import os
import warnings
import numpy as np
import pandas as pd

from sklearn.impute import SimpleImputer
from sklearn.preprocessing import StandardScaler, OneHotEncoder, LabelEncoder

warnings.filterwarnings("ignore")


class DataPreprocessor:

    def __init__(self, data_dir="."):
        self.data_dir = data_dir

        self.categorical_features = [
            "workclass", "education", "marital.status", "occupation",
            "relationship", "race", "sex", "native.country"
        ]

        self.numerical_features = [
            "age", "fnlwgt", "education.num",
            "capital.gain", "capital.loss", "hours.per.week"
        ]

        self.cat_imputer = None
        self.num_imputer = None
        self.scaler = None
        self.encoder = None
        self.label_encoders = None
        self.feature_columns = None

    def load_data(self):
        print("Loading data...")
        train_path = os.path.join(self.data_dir, "traindata.csv")
        test_path = os.path.join(self.data_dir, "testdata.csv")
        label_path = os.path.join(self.data_dir, "trainlabel.txt")

        train_df = pd.read_csv(train_path)
        test_df = pd.read_csv(test_path)

        with open(label_path, "r") as f:
            labels = [int(x.strip()) for x in f]

        print(f"\nTraining data shape: {train_df.shape}")
        print(f"Testing data shape: {test_df.shape}")

        pos_cnt = sum(labels)
        neg_cnt = len(labels) - pos_cnt

        print(f"\nTotal positive labels: {pos_cnt}")
        print(f"Total negative labels: {neg_cnt}")
        print(f"Positive class ratio: {pos_cnt / len(labels):.4f}")

        return train_df, test_df, labels

    def analyze_missing_values(self, train_df, test_df):
        print("Analysing missing values...")

        def _count(df, cols):
            result = {}
            for c in cols:
                if c not in df.columns:
                    continue
                if df[c].dtype == object:
                    num = (df[c] == "?").sum()
                else:
                    num = df[c].isna().sum()
                if num > 0:
                    result[c] = num
            return result

        tr_cat = _count(train_df, self.categorical_features)
        tr_num = _count(train_df, self.numerical_features)
        te_cat = _count(test_df, self.categorical_features)
        te_num = _count(test_df, self.numerical_features)

        if tr_cat:
            print(f"Training categorical features with missing values: {tr_cat}")
        if tr_num:
            print(f"Training numerical features with missing values: {tr_num}")
        if te_cat:
            print(f"Testing categorical features with missing values: {te_cat}")
        if te_num:
            print(f"Testing numerical features with missing values: {te_num}")

    def handle_missing_values(self, train_df, test_df, fit=True):
        print("Handling missing values with mode imputation...")

        if fit:
            self.cat_imputer = SimpleImputer(strategy="most_frequent")
            train_df[self.categorical_features] = self.cat_imputer.fit_transform(
                train_df[self.categorical_features]
            )
        else:
            train_df[self.categorical_features] = self.cat_imputer.transform(
                train_df[self.categorical_features]
            )

        test_df[self.categorical_features] = self.cat_imputer.transform(
            test_df[self.categorical_features]
        )

        if fit:
            self.num_imputer = SimpleImputer(strategy="most_frequent")
            train_df[self.numerical_features] = self.num_imputer.fit_transform(
                train_df[self.numerical_features]
            )
        else:
            train_df[self.numerical_features] = self.num_imputer.transform(
                train_df[self.numerical_features]
            )

        test_df[self.numerical_features] = self.num_imputer.transform(
            test_df[self.numerical_features]
        )

        print("Missing values handled successfully")
        return train_df, test_df


    def one_hot_encoding(self, train_df, test_df, fit=True):
        print("Encoding categorical features...")

        cat_cols = train_df.select_dtypes(include=["object"]).columns.tolist()
        print(f"Detected categorical columns: {cat_cols}")

        if fit:
            self.encoder = OneHotEncoder(
                sparse_output=False, handle_unknown="ignore"
            )
            train_arr = self.encoder.fit_transform(train_df[cat_cols])
        else:
            train_arr = self.encoder.transform(train_df[cat_cols])

        feature_names = self.encoder.get_feature_names_out(cat_cols)
        train_encoded = pd.DataFrame(train_arr, columns=feature_names, index=train_df.index)

        rest_cols = [c for c in train_df.columns if c not in cat_cols]
        for c in rest_cols:
            train_encoded[c] = train_df[c].values

        test_arr = self.encoder.transform(test_df[cat_cols])
        test_encoded = pd.DataFrame(test_arr, columns=feature_names, index=test_df.index)

        for c in rest_cols:
            test_encoded[c] = test_df[c].values

        print(f"Encoded to {len(train_encoded.columns)} features")
        return train_encoded, test_encoded

    def label_encoding(self, train_df, test_df, fit=True):
        print("Encoding categorical features...")

        cat_cols = train_df.select_dtypes(include=["object"]).columns.tolist()
        print(f"Detected categorical columns: {cat_cols}")

        if fit:
            self.label_encoders = {}
            for c in cat_cols:
                le = LabelEncoder()
                merged = pd.concat([train_df[c], test_df[c]])
                le.fit(merged)
                train_df[c] = le.transform(train_df[c])
                self.label_encoders[c] = le
        else:
            for c in cat_cols:
                train_df[c] = self.label_encoders[c].transform(train_df[c])

        for c in cat_cols:
            test_df[c] = self.label_encoders[c].transform(test_df[c])

        print("Label encoding completed")
        return train_df, test_df

    def feature_engineering(self, train_df, test_df):
        print("Performing feature engineering...")

        bins = [0, 18, 35, 50, 65, 100]
        labels = ["lt18", "18-35", "35-50", "50-65", "gt65"]

        for df in (train_df, test_df):
            df["age_group"] = pd.cut(df["age"], bins=bins, labels=labels).astype(str)

        print("Created feature: age_group")

        for df in (train_df, test_df):
            df["capital_total"] = df["capital.gain"] + df["capital.loss"]

        print("Created feature: capital_total")

        for df in (train_df, test_df):
            df["has_capital"] = (df["capital_total"] > 0).astype(int)

        print("Created feature: has_capital")
        return train_df, test_df

    def remove_low_predictive_features(self, train_df, test_df, features_to_remove=("fnlwgt",)):
        print("Removing features with low predictive power...")

        for feat in features_to_remove:
            if feat in train_df.columns:
                train_df = train_df.drop(columns=[feat])
                if feat in test_df.columns:
                    test_df = test_df.drop(columns=[feat])
                print(f"Removed feature: {feat}")

        return train_df, test_df

    def scale_numerical_features(self, train_df, test_df, fit=True):
        print("Scaling numerical features...")

        num_cols = train_df.select_dtypes(include=[np.number]).columns.tolist()

        if fit:
            self.scaler = StandardScaler()
            train_df[num_cols] = self.scaler.fit_transform(train_df[num_cols])
        else:
            train_df[num_cols] = self.scaler.transform(train_df[num_cols])

        test_df[num_cols] = self.scaler.transform(test_df[num_cols])

        print(f"Scaled {len(num_cols)} numerical features")
        return train_df, test_df

    def preprocess(
        self,
        feature_engineering=False,
        remove_low_predictive=False,
        one_hot_encoding=True,
        scale=True,
        fit=True,
    ):
        train_df, test_df, y = self.load_data()

        self.analyze_missing_values(train_df, test_df)
        train_df, test_df = self.handle_missing_values(train_df, test_df, fit)

        if feature_engineering:
            train_df, test_df = self.feature_engineering(train_df, test_df)

        if remove_low_predictive:
            train_df, test_df = self.remove_low_predictive_features(train_df, test_df)

        if one_hot_encoding:
            train_df, test_df = self.one_hot_encoding(train_df, test_df, fit)
        else:
            train_df, test_df = self.label_encoding(train_df, test_df, fit)

        if scale:
            train_df, test_df = self.scale_numerical_features(train_df, test_df, fit)

        self.feature_columns = train_df.columns.tolist()

        print("Preprocessing Complete!")
        return train_df, y, test_df
