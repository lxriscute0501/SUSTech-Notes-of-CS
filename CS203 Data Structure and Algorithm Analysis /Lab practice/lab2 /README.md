# lab 2

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Interesting number

### Description

You are given $n (n \geq 3)$ numbers. 
Lanran thinks that the third largest number is interesting, 
but lanran does not like multiple numbers with the same value. 

If there are more than one numbers equal to the third largest number, please output `wa` (without quotes), otherwise please print it out.

### Input

There are multiple testcases.

The first line of the input contains a single integer $T (1 \leq T \leq 10)$, indicates the number of the testcases.

Then $T$ testcases follow:

For each testcase, the first line contains a single integer $n (3 \leq n \leq 100)$.

The second line contains $n$ integers $a_1, a_2, ..., a_n \ (0 \leq a_i \leq 100)$, separated by a space.

### Output

Output $T$ integers for the answers of $T$ testcases.

### Sample Input
```
7
3
1 1 1
4
2 3 3 3
5
1 2 3 4 2
3
3 1 2
4
4 9 13 13
5
20 1 20 1 9
10
6 7 8 9 10 1 2 3 4 5
```

### Sample Output
```
wa
wa
wa
1
9
9
8
```

<br>

## B. Merge

### Description

**Merge** is an essiential step when you implement **Merge Sort Algorithm**. That is, given two sorted arrays, you need to combine them to one sorted array. Lanran gets puzzled at how to implement Merge operation, could you help him?


### Input

The first line contains an integer $T (1 \leq T \leq 10)$, representing the number of test cases. 

In each test case, the first line will be two integers $n$ and $m$, which are the lengths of the two sorted arrays in the ascending order.   

The second line will be the terms of the first array.  

The third line will be the terms of the second array. ($1 \leq n, m \leq 100000$, the combat values will in the range of $[0, 10^9]$)

### Output

For each test case, print the combined array.

### Sample Input
```
1
4 5
1 2 3 4
1 2 3 4 5
```

### Sample Output
```
1 1 2 2 3 3 4 4 5
```

<br>

## C. Only 3-sum

### Description

Given $n$ numbers $a_1, a_2, ..., a_n$, and a lucky number $m$, please output the number of triple $(i, j, k)$, satisfying $a_i + a_j + a_k = m \ (i < j < k)$.

### Input

The first line of the input contains two integers $n, m  \ (1 \leq n \leq 3000, 1 \leq m \leq 10^9)$.

The second line contains $n$ integers $a_1, a_2, ..., a_n \ (1 \leq a_i \leq 10^9)$.

### Output

Output one integer indicates the answer.

### Sample Input
```
4 9
1 3 5 3
```

### Sample Output
```
2
```

<br>

## D. Arrange Seats in a Round Table

### Description

Suppose there are $n$ students in CS203 DSAA, the height of each student is $a_i (1 \leq i \leq n)$. 

You are asked to arrange these students to the seats (i.e., $b_1, b_2, ..., b_n$) in a round table such that the minimum value, denoted by $k$, of the medium height of every three students in consecutive seats (i.e., $b_i, b_{i \mod (n+1)}, b_{(i+1) \mod (n+1)}$) is maximized.

Please print $k$ and the height of the student in seats $b_1, b_2, ..., b_n$.

If it has more than one possible solution, print the height of the student in seats $b_1, b_2, ..., b_n$ with smallest [lexicographical order](https://en.wikipedia.org/wiki/Lexicographic_order). 

### Input

There will be two lines.

The first line will be an integer $n$.

The second line will be $n$ integers $a_1, a_2, ..., a_n$.

For all test cases, $3 \leq n \leq 10^6, 0 \leq a_i \leq 10^9$.

### Output

There will be two lines.

The first line prints the value of $k$.

The second line prints the height of the student in seats $b_1, b_2, ..., b_n$.

### Sample Input
```
6
6 5 4 3 2 1
```

### Sample Output
```
3
1 3 4 2 5 6
```

<br>

## E. Pay to swap

### Description

Given a random sequence $\{a\}$ with $n$ distinct elements. 

You can swap two adjacent elements each time, and the cost is the smaller one.

You want to know the minimum cost to sort the sequence to a ascending sequence.

For example, if you swap $(a_i, a_{i+1})$, then the cost of this operation is $\min(a_i, a_{i+1})$.


### Input

The first line of the input contains one integer $n$.

The second line contains $n$ integers $a_1, a_2, ..., a_n$.

For all cases, $1 \leq n \leq 10^5, 0 \leq a_i \leq 10^9$.


### Output

Output one integer indicates the minimum cost.

### Sample Input
```
5
1 3 2 4 5
```

### Sample Output
```
2
```

<br>

## F. YYJ's magic beads

### Description

YYJ has many magic beads with two colors, red and blue. If a red bead is on the left of a blue bead and they are next to each other, they will disappear and release $1$ unit of magic power. 

YYJ has $n$ strings of beads, each string is consists of $a_i$ blue beads on the left and $b_i$ red beads on the right (to make sure they will disappear). Note that $a_i$ and $b_i$ can be zero.

YYJ now wants to connect these strings in some order and she is wondering how many units of magic power she can get at most.

### Input

The first line contains an integer $T$, indicating the number of test cases. For each test case:

The first line contains an integer $n (1 \leq n \leq 100,000)$, indicating the number of string of beads.

Each of the next $n$ lines contains two integers $a_i, b_i \ (1 \leq a_i + b_i \leq 10,000)$.

It is guaranteed that $\sum (a_i + b_i) \leq 500,000$.

### Output

Output one integer, indicating the answer.

### Sample Input
```
2
2
1 2
2 1
2
1 3
2 1
```

### Sample Output
```
2
2
```

### Hint

We use $R$ to denote red beads and $B$ to denote blue beads.

We have two strings: $BRR$, $BBR$ at first.

The string after connection is: $BRRBBR$, which can gain $2$ units of magic power.

