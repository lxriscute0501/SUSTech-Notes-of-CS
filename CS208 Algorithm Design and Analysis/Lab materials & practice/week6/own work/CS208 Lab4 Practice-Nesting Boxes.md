# CS208 Lab4 Practice-Nesting Boxes

12312110 李轩然

**DDL: Apr.13**

## Description

A ``d``-dimensional box with dimensions (``x_1, x_2, ..., x_d``) nests within another box with dimensions (``y_1, y_2, ..., y_d``). If there exists a permutation ``π`` on ``\{1, 2, ..., d\}`` such that ``x_{π(1)} < y_1``, ``x_{π(2)} < y_2``, ``...``, ``x_{π(d)} < y_d``: 1. Is the nesting relation transitive? Prove it.2. Design an efficient method to determine whether one ``d``-dimensional box nests inside another. Describe the algorithm’s steps, state its time complexity, and prove its correctness.3. Suppose that you are given a set of ``n`` ``d``-dimensional boxes ``\{B_1, B_2, ..., B_n\}``. Give an efficient algorithm to find the longest sequence ``\{B_{i1}, B_{i2}, ..., B_{ik}\}`` of boxes such that ``B_{ij}`` nests within  ``B_{ij+1}`` for ``j = 1, 2, ..., k − 1``. Express the running time of your algorithm in terms of ``n`` and ``d``.


## Requirement

This week's practice does not require writing code. Instead, you need to describe the algorithmic steps (or provide pseudocode) and clearly explain the proof process.Expected Output:- A structured algorithm description (or pseudocode).- A formal correctness proof (e.g., by induction, contradiction, or logical reasoning). 
 
## Analysis

### 1. The nesting relation is transitive

**Proof:** Suppose we have three boxes named ``A``, ``B`` and ``C`` with dimensions: 
```math
A = (a_1, a_2, ..., a_d)
```
```math
B = (b_1, b_2, ..., b_d)
```
```math
C = (c_1, c_2, ..., c_d)
```

Assume ``A`` nests within ``B``, ``B`` nests within ``C``, thus we need to prove that ``A`` nests within ``C``.

By definition, there exists a bijection ``\alpha: \{1, 2, ..., d\} \\\rightarrow \{1, 2, ..., d\}`` such that ``b_{\alpha(1)} < c_1``, ``c_{\alpha(2)} < y_2``, ``...``, ``b_{\alpha(d)} < c_d``; there also exists a bijection ``\beta: \{1, 2, ..., d\} \\\rightarrow \{1, 2, ..., d\}`` such that ``a_{\beta(1)} < b_1``, ``a_{\beta(2)} < b_2``, ``...``, ``a_{\beta(d)} < b_d``.

According to discrete mathematics, ``\beta \circ \alpha`` is also bijective from ``\{1, 2, ..., d\}`` to ``\{1, 2, ..., d\}``, and: 
```math
a_{\beta \circ \alpha(1)} < b_{\alpha(1)} < c_1, a_{\beta \circ \alpha(2)} < b_{\alpha(2)} < c_2, ..., a_{\beta \circ \alpha(d)} < b_{\alpha(d)} < c_d
```which means ``A`` nests within ``C`` by definiton. Then we are done.

### 2. Sorting

Suppose we need to determin that ``X`` nests within ``Y`` with dimensions:
```math
X = (x_1, x_2, ..., x_d)
```
```math
Y = (y_1, y_2, ..., y_d)
```

We just sort the dimensions of ``X`` and ``Y`` in ascending order: 
```math
X = (x'_1, x'_2, ..., x'_d), x'_1 \leq x'_2 \leq ... \leq x'_d
```
```math
Y = (y'_1, y'_2, ..., y'_d), y'_1 \leq y'_2 \leq ... \leq y'_d
```

Then ``X`` nests within ``Y`` iff:
```math
x'_1 < y'_1, x'_2 < y'_2, ..., x'_d < y'_d
```

The total time complexity will be ``O(dlogd)`` because of sorting.

**Correctness proof:** Assume ``X`` nests within ``Y``, but there exists ``i \in \{1, 2, ..., d\}``, such that ``x'_i \geq y'_i`` after sorting. Because ``y'_1 \leq y'_2 \leq ... \leq y'_d``, then there are ``n-i+1`` elements in ``X`` that not smaller than ``y'_i``. However, there are only ``n-i`` elements in ``Y`` that greater than ``y'_i``, thus there must exist at least one element in ``X``, let's say ``x'_j (i \leq j \leq d)``, matches ``y'_i`` or smaller element in ``Y``, let's say ``y'_k (1 \leq k \leq i)``, then ``x'_j \geq y'_k``, which means ``X`` does not nest within ``Y`` and makes a contridiction. So the algorithm is correct.

### 3. Greedy

Still sort the dimensions of each ``B_i (1 \leq i \leq n)`` in ascending order:
```math
B_i = (b'_{i1}, b'_{i2}, ..., b'_{id}), b'_{i1} \leq b'_{i2} \leq ... \leq b'_{id}
```
then sort ``B_1, B_2, ..., B_n`` by sorting ``b'_{11}, b'_{21}, ..., b'_{n1}`` (if equals, then compare ``b'_{i2}``......) in ascending order:
```math
B_1, B_2, ..., B_n (b'_{11} \leq b'_{21} \leq ... \leq b'_{n1})
```

In this case, we don't know whether the largest box, which is ``B_n``, can be chosen, so we need to list every ``B_i`` that probably be the largest. Define ``f_i (1 \leq i \leq n)`` that means when the largest box is ``B_i``, the number of boxes we can choose at most. Then the answer will be ``\max \{f_1, f_2, ..., f_n\}`` but not ``f_n``.

Clearly, ``f_1 = 1``, and for ``2 \leq i \leq n``, we need to find a box that can be nested within ``B_i``, let's say ``B_j``, and its **``f_j`` is the largest**. Then add ``B_i`` to this sequence, thus ``f_i = f_j + 1``. The pseudocode is:
```pascal
f[1] = 1
for i : 2 to n
    temp = 0
    for j : i-1 downto 1
    if (B[j] nests within B[i])
        temp = max(temp, f[j])
    f[i] = temp + 1
    
return max{f[i]}
```

**Running time:** The double for-loop will cost ``O(n^2)`` time, and every judgement (``B_j`` nests within ``B_i``) will do comparation for ``d`` times, and sorting will cost ``O(ndlogn + ndlogd)`` time, thus the total time complexity will be ``O(n^2d)``.

**Correctness proof:** For every ``B_i``, we guarantee that ``f_i`` is the maximum of the number of boxes we can choose when ``B_i`` is the largeset, and ``f_1, f_2, ..., f_n`` lists all the possibilities of ``\{B_n\}`` sequence that might be the longest, thus the answer must be the longest length.
