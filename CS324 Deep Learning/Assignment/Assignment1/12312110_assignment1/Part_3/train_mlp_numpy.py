import argparse
import numpy as np
from Part_2.mlp_numpy import MLP
from Part_2.modules import CrossEntropy
from sklearn.datasets import make_moons
from sklearn.model_selection import train_test_split

# Default constants
DNN_HIDDEN_UNITS_DEFAULT = '20'
LEARNING_RATE_DEFAULT = 1e-2
MAX_EPOCHS_DEFAULT = 1500  # adjust if you use batch or not
EVAL_FREQ_DEFAULT = 10
SGD_DEFAULT = True
BATCH_SIZE_DEFAULT = 1

def accuracy(predictions, targets):
    """
    Computes the prediction accuracy, i.e., the percentage of correct predictions.

    Args:
        predictions: 2D float array of size [number_of_data_samples, n_classes]
        targets: 2D int array of size [number_of_data_samples, n_classes] with one-hot encoding

    Returns:
        accuracy: scalar float, the accuracy of predictions as a percentage.
    """
    # TODO: Implement the accuracy calculation
    # Hint: Use np.argmax to find predicted classes, and compare with the true classes in targets
    p = np.argmax(predictions, axis=1)
    true_class = np.argmax(targets, axis=1)
    return np.mean(p == true_class)


def shuffle_and_batch(X, y, batch_size):
    shuffle_index = np.random.permutation(len(X))
    X_shuffled = X[shuffle_index]
    y_shuffled = y[shuffle_index]

    num_batches = len(X) // batch_size
    batches = []

    for i in range(num_batches):
        X_batch = X_shuffled[i * batch_size:(i + 1) * batch_size]
        y_batch = y_shuffled[i * batch_size:(i + 1) * batch_size]
        batches.append((X_batch, y_batch))

    if len(X) % batch_size != 0:
        X_batch = X_shuffled[num_batches * batch_size:]
        y_batch = y_shuffled[num_batches * batch_size:]
        batches.append((X_batch, y_batch))

    return batches


def train(dnn_hidden_units, learning_rate, max_steps, eval_freq, sgd, batch_size):
    """
    Performs training and evaluation of MLP model.

    Args:
        dnn_hidden_units: Comma separated list of number of units in each hidden layer
        learning_rate: Learning rate for optimization
        max_steps: Number of epochs to run trainer
        eval_freq: Frequency of evaluation on the test set
        NOTE: Add necessary arguments such as the data, your model...
    """
    # TODO: Load your data here
    dnn_hidden_units = [int(x) for x in dnn_hidden_units.split(',')]
    X, y = make_moons(n_samples=1000, shuffle=True)
    n_values = np.max(y) + 1
    y = np.eye(n_values)[y]
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, shuffle=True)

    # TODO: Initialize your MLP model and loss function (CrossEntropy) here
    model = MLP(n_inputs=2, n_hidden=dnn_hidden_units, n_classes=2)
    loss = CrossEntropy()

    for step in range(max_steps):
        # TODO: Implement the training loop
        if sgd == False:
            y = model.forward(X_train)
            l = loss.forward(y, y_train)
            dout = loss.backward(y, y_train)
            model.backward(dout)
            model.head.params['weight'] = model.head.params['weight'] - learning_rate * model.head.grads['weight']
            model.head.params['bias'] = model.head.params['bias'] - learning_rate * model.head.grads['bias']
            for linear in model.linears:
                linear.params['weight'] = linear.params['weight'] - learning_rate * linear.grads['weight']
                linear.params['bias'] = linear.params['bias'] - learning_rate * linear.grads['bias']
        else:
            batches = shuffle_and_batch(X_train, y_train, batch_size=batch_size)
            for X, y in batches:
                X = X.reshape((1, -1))
                y_hat = model.forward(X)
                l = loss.forward(y_hat, y)
                dout = loss.backward(y_hat, y)
                model.backward(dout)
                model.head.params['weight'] = model.head.params['weight'] - learning_rate * model.head.grads['weight']
                model.head.params['bias'] = model.head.params['bias'] - learning_rate * model.head.grads['bias']
                for linear in model.linears:
                    linear.params['weight'] = linear.params['weight'] - learning_rate * linear.grads['weight']
                    linear.params['bias'] = linear.params['bias'] - learning_rate * linear.grads['bias']

        if step % eval_freq == 0 or step == max_steps - 1:
            # TODO: Evaluate the model on the test set
            # 1. Forward pass on the test set
            # 2. Compute loss and accuracy
            y = model.forward(X_test)
            print(f"Step: {step}, Loss: {l}, Accuracy: {accuracy(y, y_test)}")

    print("Training complete!")


def main():
    """
    Main function.
    """
    # Parsing command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument('--dnn_hidden_units', type=str, default=DNN_HIDDEN_UNITS_DEFAULT,
                        help='Comma separated list of number of units in each hidden layer')
    parser.add_argument('--learning_rate', type=float, default=LEARNING_RATE_DEFAULT,
                        help='Learning rate')
    parser.add_argument('--max_steps', type=int, default=MAX_EPOCHS_DEFAULT,
                        help='Number of epochs to run trainer')
    parser.add_argument('--eval_freq', type=int, default=EVAL_FREQ_DEFAULT,
                        help='Frequency of evaluation on the test set')
    parser.add_argument('--sgd', type=bool, default=SGD_DEFAULT,
                        help='Whether using SGD or not')
    parser.add_argument('--batch_size', type=int, default=BATCH_SIZE_DEFAULT,
                        help='The batch size of SGD')
    FLAGS = parser.parse_known_args()[0]

    train(FLAGS.dnn_hidden_units, FLAGS.learning_rate, FLAGS.max_steps, FLAGS.eval_freq, FLAGS.sgd, FLAGS.batch_size)


if __name__ == '__main__':
    main()
