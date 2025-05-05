# Lab 8 A. FFT

## Description

Given a finite sequence $s$ of length $2^{N}$ of equally-spaced samples of a function, find the result of its Discrete Fourier Transform.

## Input Format

The first line contains an integer $N$.

The following line contains $2^N$ decimal numbers $s_0, s_1, \dots, s_{2^N - 1}$, representing the equally-spaced samples.

## Output Format

Output $2^N$ lines. For the $i^{th}$ line, output the magnitude of the $(i-1)^{th}$ complex in the result.

Your output will be considered correct, if the absolute or relative error between your output and the answer is less than $10^{-6}$.

## Samples

### Sample 1 Input

```
2
0 1 0 -1
```

### Sample 1 Output

```
0.0000000000
2.0000000000
0.0000000000
2.0000000000
```

### Sample 2 Input

```
4
2 3 5 2 1 3 4 5 2 3 1 6 4 2 1 2
```

### Sample 2 Output

```
46.0000000000
3.9912977684
2.6131259298
12.1870646827
4.4721359550
5.8052434198
1.0823922003
6.4687050775
6.0000000000
6.4687050775
1.0823922003
5.8052434198
4.4721359550
12.1870646827
2.6131259298
3.9912977684
```

## Limitations & Hints

| test case | N | input abs upper bound |
|:---:|:---:|:---:|
|1|1|10000|
|2|2|10000|
|3|3|30000|
|4|4|100000|
|5|5|100000|
|6|6|100000|
|7|7|200000|
|8|8|300000|
|9|9|300000|
|10|10|4000000|
|11|11|4000000|
|12|12|5000000|
|13|13|5000000|
|14|14|6000000|
|15|15|6000000|
|16|16|7000000|
|17|17|8000000|
|18|17|9000000|
|19|17|10000000|
|20|17|10000000|

<br>

# Lab 8 B. Seat Selection

## Description

Xiaofeng is a student at SUSTech, and he often goes to the library to study. In the library, seats are arranged in a two-dimensional grid, with each seat represented by a coordinate. One day he entered the library and wanted to know how far apart the two nearest vacant seats in the entire library were.

Your task is to help Xiaofeng choose the best seat, i.e., find the smallest distance between two vacant seats in the library.

## Input Format

* The first line contains an integer $n$, representing the number of vacant seats in the library, where $2 \leq n \leq 200000$.
* The next $n$ lines contain two floating-point numbers $x$ and $y$, representing the coordinates of a vacant seat.

## Output Format

* Output a single floating-point number representing the shortest distance between two vacant seats, rounded to 4 decimal places.

## Example

**Input:**

```
4

0 0

1 1

2 2

3 3
```

**Output:**

```
1.4142
```

## Constraints

* The coordinates $x$ and $y$ satisfy $ 0 \leq x, y \leq 10^9$.
