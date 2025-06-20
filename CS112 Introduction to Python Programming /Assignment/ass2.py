# 1
def trapezium_area():
    a = float(input("Enter the first base (a): "))
    b = float(input("Enter the second base (b): "))
    h = float(input("Enter the height (h): "))

    area = 0.5 * (a + b) * h

    print("Area of trapezium:", area)

trapezium_area()


# 2
import numpy as np

A = np.random.randn(20, 50)

result1 = A + A

result2 = np.dot(A, A.T)

result3 = np.dot(A.T, A)

print("A+A:\n", result1)
print("AA^T:\n", result2)
print("A^TA:\n", result3)


# 3
import numpy as np

A = np.array([
    [1, -2, 9, 13],
    [-5, 2, 6, -7],
    [4, 8, -4, -2],
    [8, 5, -7, 1]
])

b = np.array([1, -3, -2, 5])

A_inv = np.linalg.inv(A)
x = np.dot(A_inv, b)

print("x1 =", x[0])
print("x2 =", x[1])
print("x3 =", x[2])
print("x4 =", x[3])


# 4
import numpy as np
from scipy.integrate import quad

def integrand(x):
    denominator = (np.exp(x) + x + 1) ** 2 + np.pi ** 2
    return 1 / denominator

result, error = quad(integrand, -np.inf, np.inf)

print("Numerical integration result:", result)
print("Estimated error:", error)

