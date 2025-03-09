# lab1
Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Binary Search

### Description

Given a nondecreasing sequence a with length `n`. We want to know whether integer `x` in the array a or not.  

### Input

The 1st line is a positive integer `n (1 <= n <= 100000)`.

The 2nd line contains `n` integers: `a1, a2, ..., an`. For each `ai (0 <= ai <= 1e9)`.

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

## D. [Median I] Factorial Magic

### Description

Neko is a freshman at SUSTech and he is good at fractorial problems. He wants to challenge you to see if you can solve the following problem: Caculate the value of:
```math
((n!)!)! &ensp; mod &ensp; m
```

### Input

There is only one line contains two integers `n, m (0 <= n <= 1e9, 1 <= m <= 1e9)`.

### Output

Print the value of `((n!)!)! (mod m)`.

### Sample Input
```
1 2019
```

### Sample Output
```
1
```

<br>

## E. Sport Meeting

### Description

Annual Sport meeting in S University starts again. This year, the rule of relay running is modified by president of sports department. The rule after modification is as follows:

The total length of the relay running is `L (1 <= L <= 1e9)`. There are `n (0 <= n <= 500000)` possible places to place a new racer (there is a race in the start line). Every racer runs to the next racer in front of him and the final racer runs to the finish line. But the number of racers is limited to a number `m (1 <= m <= n + 1)`. Team of class 1788 does not want any of them to run too much. Therefore, they wish to minimize the longest distance that any one of them needs to run.

### Input

The input contains several test cases. The first line of each case contains three positive integer `L`, `n`, and `m`. 
Then `n` lines follow. Each stands for the distance from the start line to the `n-th` possible place to place new racer, two places will not be the same.

### Output

For each case, output an integer which is the minimium longest distance that any one of class 1788 needs to run.

### Sample Input
```
6 1 2
2
25 3 3
11 
2
18
```

### Sample Output
```
4
11
```

### Hint

**Arrays.sort()** is allowed in this problem.

Since we don't know how many testcases are there, we need to determine whether the input is finished. Here are the sample code:

**JAVA:**
```java
public static void main(String[] args) {
        Scanner scanner =newScanner(System.in);
        while(scanner.hasNext()) {
             //to do
        }        
    }
```


**C:**
```c
#include<stdio.h>
int main()
{
    int n;
    while (scanf("%d", &n) != EOF) {
        // do something
    }
    return 0;
}
```


**C++:**
```cpp
#include<iostream>
int main()
{
    int n;
    while (std::cin >> n) {
        // do something
    }
    return 0;
}
```


<br>

## F. Median I

### Description

Given two nondecreasing sequences `a` and `b`, and their length are both `n`. What's the median of after combining the subarray `a[l..r]` and subarray `b[l..r]`?

Subarray `a[l..r]` is a sub-array of `a`, it includes `al, al+1, ..., ar` for `1 <= l <= r <= n`, its length is `r − l + 1`.

You’d like to determine the **median** of this set of `2k (k = r − l + 1)` values, which we will define here to be the `k-th` smallest value. For example: `median([1, 2, 3, 4]) = 2`.

### Input

The 1st line contains two positive integers `n (1 <= n <= 100000)` and `T (1 <= T <= 100000)` which is the number of testcase.

The 2nd line contains `n` integers: `a1, a2, ..., an`. For each `ai`, `0 <= ai <= 1e9`.

The 3rd line contains `n` integers: `b1, b2, ..., bn`. For each `bi`, `0 <= bi <= 1e9`.

Then `T` lines follow. Each line contains two integers `l` and `r` `(1 <= l <= r <= n)` for a test case.

### Output

Output `T` lines. Each line contains an integer `ans`, the median of after combining the subarray `a[l..r]` and subarray `b[l..r]`.


### Sample Input
```
5 2
1 3 5 7 9
2 3 4 5 6
5 5
1 5
```

### Sample Output
```
6
4
```

### Hint

The correspond solutions to the sample is:      
(1) `a5 = 9`, `b5 = 6`, after combining is `[9 6]`, the median is `6`.      
(2) Combine `a` and `b` then sort it can get `[1 2 3 3 4 5 5 6 7 9]`, the median is `4`.      

