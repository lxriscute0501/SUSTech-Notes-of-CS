import numpy as np


class Perceptron(object):

    def __init__(self, n_inputs, max_epochs=20, learning_rate=0.01):
        """
        Initializes the perceptron object.
        - n_inputs: Number of inputs.
        - max_epochs: Maximum number of training cycles.
        - learning_rate: Magnitude of weight changes at each training cycle.
        - weights: Initialize weights (including bias).
        """
        self.n_inputs = n_inputs
        self.max_epochs = max_epochs
        self.learning_rate = learning_rate
        self.weights = np.zeros(n_inputs + 1)

    def forward(self, input_vec):
        """
        Predicts label from input.
        Args:
            input_vec (np.ndarray): Input array of training data, input vec must be all samples
        Returns:
            int: Predicted label (1 or -1) or Predicted lables.
        """
        label_0 = np.ones([input_vec.shape[0], 1])
        matrix = np.hstack((input_vec, label_0))
        y = np.dot(self.weights, matrix.T)
        y = np.array([1 if i >= 0 else -1 for i in y])
        return y

    def train(self, training_inputs, labels):
        """
        Trains the perceptron.
        Args:
            training_inputs (list of np.ndarray): List of numpy arrays of training points.
            labels (np.ndarray): Array of expected output values for the corresponding point in training_inputs.
        """
        # we need max_epochs to train our model
        for _ in range(self.max_epochs):
            """
                What we should do in one epoch ? 
                you are required to write code for 
                1.do forward pass
                2.calculate the error
                3.compute parameters' gradient 
                4.Using gradient descent method to update parameters(not Stochastic gradient descent!,
                please follow the algorithm procedure in "perceptron_tutorial.pdf".)
            """
            p = self.forward(training_inputs)
            mask = p * labels < 0
            X, y = training_inputs[mask], labels[mask]
            grad = -np.sum(X * y.reshape(y.shape[0], 1)) / training_inputs.shape[0]
            self.weights = self.weights - self.learning_rate * grad
