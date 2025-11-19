import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score

from perceptron import Perceptron


def generate(mean=2, cov=1):
    np.random.seed(11)
    mean1, cov1 = [mean, mean], [[cov, cov], [cov, cov]]
    mean2, cov2 = [-mean, -mean], [[cov, -cov], [-cov, cov]]

    points1 = np.random.multivariate_normal(mean1, cov1, 100)
    points2 = np.random.multivariate_normal(mean2, cov2, 100)

    X = np.vstack((points1, points2))
    y = np.hstack((np.array([-1 for _ in range(100)]), np.ones(100)))
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, stratify=y)

    return X_train, X_test, y_train, y_test


if __name__ == '__main__':
    means, covs = [20, 2, 0.2, 0.02], [10, 1, 0.1, 0.01]
    for mean in means:
        for cov in covs:
            model = Perceptron(n_inputs=2, max_epochs=20, learning_rate=0.01)
            X_train, X_test, y_train, y_test = generate(mean, cov)
            model.train(X_train, y_train)
            p = model.forward(X_test)
            print(f'mean={mean}, cov={cov}, Accuracy: {accuracy_score(p, y_test)}')