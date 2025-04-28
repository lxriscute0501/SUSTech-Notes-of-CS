# Lab7 A. Longest Substring

## Description

At the SUSTech library, a student recorded a sequence of reading notes. The notes consist of a string $s$ where each character represents the first letter of a keyword from their reading.

Now, the student wants to extract a continuous segment of this $s$ to present to their advisor, with the following requirement:

In the chosen substring, **each character must appear at least $k$ times**.

Your task is to determine the **length of the longest valid substrisng** that satisfies this condition. If no such substring exists, return 0.

## Input Format

* A string $s$, consisting only of lowercase English letters.
* An integer $s$, the minimum number of occurrences required for each character in the selected segment.


## Output Format

* An integer representing the length of the longest valid substring.


## Examples

### Example 1

```
Input: ddbbb 3
Output: 3
Explanation: The substring "bbb" is valid since 'b' appears 3 times.
```

### Example 2

```
Input: acabbab 2
Output: 5
Explanation: The substring "abbab" is valid since 'a' appears 2 times and 'b' appears 3 times.
```


## Constraints

* $1 \leq s.length \leq 10^4$
* $1 \leq k \leq 10^5$
* $s$ consists of lowercase English letters only

<br>

# Lab7 B. Overlapping Rectangles

## Description

You are given a list of rectangles on a 2D plane. Each rectangle is represented by a triplet $[left_i, \ right_i, \ height_i]$, where:

* $left_i$ is the x-coordinate of the left edge of the rectangle,
* $right_i$ is the x-coordinate of the right edge (exclusive),
* $height_i$ is the height of the rectangle.

Each rectangle extends vertically from height $0$ up to $height_i$, and spans horizontally from $left_i$ to $right_i$.\
All rectangles have a width along the x-axis and are aligned with the axes.

Your task is to compute the **outline of the combined shape** formed by these rectangles, as seen from the front (along the y-axis).\
This outline can be represented by a list of **key points** which indicate where the height of the shape changes.


## Input Format

An array $rectangles$ of shape $n \times 3$, including $left_i, \ right_i, \ height_i$.


## Output Format

A list of key points $x_1 \ y_1, x_2 \ y_2, ...$, sorted in increasing order of $x$.

Each key point represents a horizontal position $x$ where the height changes to $y$.\
Consecutive key points must not have the same height value.

The final point should represent the drop to ground level ($height = 0$) at the end of the last rectangle.


## Example

**Input:**

```
5
2 9 10
3 7 15
5 12 12
15 20 10
19 24 8
```

**Output：**

```
2 10
3 15
7 12
12 0
15 10
20 8
24 0
```

## Constraints

* $1 \leq rectangles.length \leq 10^4$
* $0 \leq left_i < right_i \leq 2^{31} - 1$
* $1 \leq height_i \leq 2^{31} - 1$
* The input array is sorted by $left_i$ in non-decreasing order.
