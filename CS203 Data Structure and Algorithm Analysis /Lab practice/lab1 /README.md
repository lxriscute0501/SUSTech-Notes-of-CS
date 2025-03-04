# lab1
Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Binary Search

### Description

Given a nondecreasing sequence a with length `n`. We want to know whether integer `x` in the array a or not.  

### Input

The 1st line is a positive integer `n (1 <= n <= 100000)`.

The 2nd line contains `n` integers: `a1, a2, ...,an`. For each `ai (0 <= ai <= 1e9)`.

The 3rd line is a positive integer `T (1 <= T <= 100000)` which is the number of test case.

Then T lines follow. Each contains an integer `x (1 <= x <= 1e9)` for a test case.

### Output

For each test case, print **"YES"** (in a single line, without quotes) if `x` is in a, otherwise print **"NO"**.


### Sample Input
```
4
1 2 3 999999
2
99 
3
```

### Sample Output
```
NO
YES
```

<br>

## B. [Easy] Simple Sum

### Description

Given `n`, please calculate 
```math
f(n) &ensp; mod &ensp; (1e9 + 7)
```
where
```math
f(n) = \sum_{n=1}^n i^3
```

### Input

The first line contains an integer `T (1 <= T <= 1000)`, representing the number of test cases.

Then `T` lines follow. Each line contains an integer `n (1 <= n <= 1e9)` for a test case.

### Output

Output `T` lines. Each line contains an integer `ans`, representing the answer.


### Sample Input
```
3
1
4
100000
```

### Sample Output
```
1
100
496501211
```

<br>

## C. Simple Problem

### Description

Given a non-decreasing array A, containing `N` positive integers `a1, a2, ...,an`. 

There are `Q` queries. Each query gives `x, y`, you are asked to check how many integers in A which are satisfying `x < ai < y`.

### Input

The first line of the input contains two integers `N` and `Q` `(1 <= N <= 1e5, 1 <= Q <= 1e5)` −− the length of A and the number of query.

The second line contains `N` integers `ai (1 <= ai <= 1e9)`.

The `i-th` of the next Q lines contains two integers `xi, yi (1 <= x <= y <= 1e9)`.

### Output

For each query, print **"NO"** (without quote) if none of integers in A satisfied the condition.

Otherwise, print **"YES"** (without quote) and the number of satisfied integers.

### Sample Input
```
5 3
1 2 3 4 5
3 5
1 6
5 5
```

### Sample Output
```
YES 1
YES 4
NO
```

<br>

