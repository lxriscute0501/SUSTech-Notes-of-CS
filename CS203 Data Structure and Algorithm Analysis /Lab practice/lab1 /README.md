# lab1
Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Binary Search

### Description

Given a nondecreasing sequence a with length $n$. We want to know whether integer $x$ in the array a or not.  

### Input

The 1st line is a positive integer $n (1 \leq n \leq 10^6)$.

The 2nd line contains $n$ integers: $a_1, a_2, ..., a_n$. For each $a_i (0 \leq a_i \leq 1e9)$.

The 3rd line is a positive integer $T (1 \leq T \leq 10^6)$ which is the number of test case.

Then T lines follow. Each contains an integer $x (1 \leq x \leq 10^9)$ for a test case.

### Output

For each test case, print **"YES"** (in a single line, without quotes) if $x$ is in $a$, otherwise print **"NO"**.


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

Given $n$, please calculate 
```math
f(n) \mod (1e9 + 7)
```
where
```math
f(n) = \sum_{n=1}^n i^3
```

### Input

The first line contains an integer $T (1 \leq T \leq 1000)$, representing the number of test cases.

Then $T$ lines follow. Each line contains an integer $n (1 \leq n \leq 10^9)$ for a test case.

### Output

Output $T$ lines. Each line contains an integer $ans$, representing the answer.


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

Given a non-decreasing array $a$, containing $n$ positive integers $a_1, a_2, ..., a_n$. 

There are $Q$ queries. Each query gives $x, y$, you are asked to check how many integers in A which are satisfying $x < a_i < y$.

### Input

The first line of the input contains two integers $N$ and $Q$ $(1 \leq N \leq 10^5, 1 \leq Q \leq 10^5)$ −− the length of $a$ and the number of query.

The second line contains $N$ integers $a_i (1 \leq a_i \leq 10^9)$.

The $i$-th of the next $Q$ lines contains two integers $x_i, y_i \ (1 \leq x \leq y \leq 10^9)$.

### Output

For each query, print **"NO"** (without quote) if none of integers in $a$ satisfied the condition.

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
((n!)!)!  \mod m
```

### Input

There is only one line contains two integers $n, m \ (0 \leq n \leq 10^9, 1 \leq m \leq 10^9)$.

### Output

Print the value of $((n!)!)! \mod m$.

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

The total length of the relay running is $L (1 \leq L \leq 10^9)$. There are $n (0 \leq n \leq 500,000)$ possible places to place a new racer (there is a race in the start line). Every racer runs to the next racer in front of him and the final racer runs to the finish line. But the number of racers is limited to a number $m (1 \leq m \leq n + 1)$. Team of class 1788 does not want any of them to run too much. Therefore, they wish to **minimize the longest distance** that any one of them needs to run.

### Input

The input contains several test cases. The first line of each case contains three positive integer $L$, $n$, and $m$. 
Then $n$ lines follow. Each stands for the distance from the start line to the $n$-th possible place to place new racer, two places will not be the same.

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

Given two nondecreasing sequences $a$ and $b$, and their length are both $n$. What's the median of after combining the subarray $a[l...r]$ and subarray $b[l...r]$?

Subarray $a[l...r]$ is a sub-array of $a$, it includes $a_l, a_{l+1}, ..., a_r$ for $1 \leq l \leq r \leq n$, its length is $r-l+1$.

You’d like to determine the **median** of this set of $2k (k = r − l + 1)$ values, which we will define here to be the $k$-th smallest value. For example: $median([1, 2, 3, 4]) = 2$.

### Input

The 1st line contains two positive integers $n (1 \leq n \leq 100,000)$ and $T (1 \leq T \leq 100,000)$ which is the number of testcase.

The 2nd line contains $n$ integers: $a_1, a_2, ..., a_n$. For each $a_i$, $0 \leq a_i \leq 10^9$.

The 3rd line contains $n$ integers: $b_1, b_2, ..., b_n$. For each $b_i$, $0 \leq b_i \leq 10^9$.

Then $T$ lines follow. Each line contains two integers $l$ and $r \ (1 \leq l \leq r \leq n)$ for a test case.

### Output

Output $T$ lines. Each line contains an integer $ans$, the median of after combining the subarray $a[l...r]$ and subarray $b[l...r]$.


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
(1) $a_5 = 9, b_5 = 6$, after combining is $[9 \ 6]$, the median is $6$.      
(2) Combine $a$ and $b$ then sort it can get $[1 \ 2 \ 3 \ 3 \ 4 \ 5 \ 5 \ 6 \ 7 \ 9]$, the median is $4$.      

