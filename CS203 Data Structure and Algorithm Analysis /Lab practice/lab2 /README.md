# lab 2

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Interesting number

### Description

You are given `n (n >= 3)` numbers. 
Lanran thinks that the third largest number is interesting, 
but lanran does not like multiple numbers with the same value. 

If there are more than one numbers equal to the third largest number, please output `'wa'` (without quotes), otherwise please print it out.

### Input

There are multiple testcases.

The first line of the input contains a single integer `T (1 <= T <= 10)`, indicates the number of the testcases.

Then `T` testcases follow:

For each testcase, the first line contains a single integer `n (3 <= n <= 100)`.

The second line contains `n` integers `a1, a2, ..., an (0 <= ai <= 100)`, separated by a space.

### Output

Output `T` integers for the answers of `T` testcases.

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

The first line contains an integer `T (1 <= T <= 10)`, representing the number of test cases. 

In each test case, the first line will be two integers `n` and `m`, which are the lengths of the two sorted arrays in the ascending order.   

The second line will be the terms of the first array.  

The third line will be the terms of the second array. (`1 <= n, m <= 100000`, the combat values will in the range of `[0, 1e9]`)

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

Given `n` numbers `a1, a2, ..., an`, and a lucky number `m`, please output the number of triple `(i, j, k)`, satisfying `ai + aj + ak = m (i < j < k)`.

### Input

The first line of the input contains two integers `n, m (1 <= n <= 3000, 1 <= m <= 1e9)`.

The second line contains `n` integers `a1, a2, ..., an (1 <= ai <= 1e9)`.

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

Suppose there are `n` students in CS203 DSAA, the height of each student is `ai (1 <= i <= n)`. 

You are asked to arrange these students to the seats (i.e., b<sub>1</sub>, b<sub>2</sub>, ..., b<sub>n</sub>) in a round table such that the minimum value, denoted by `k`, of the medium height of every three students in consecutive seats (i.e., b<sub>i</sub>, b<sub>i mod (n+1)</sub>, b<sub>(i+1) mod (n+1)</sub>) is maximized.

Please print `k` and the height of the student in seats `b1, b2, ..., bn`.

If it has more than one possible solution, print the height of the student in seats `b1, b2, ..., bn` with smallest [lexicographical order](https://en.wikipedia.org/wiki/Lexicographic_order). 

### Input

There will be two lines.

The first line will be an integer `n`.

The second line will be `n` integers `a1, a2, ..., an`.

For all test cases, `3 <= n <= 1e6, 0 <= ai <= 1e9`.

### Output

There will be two lines.

The first line prints the value of `k`.

The second line prints the height of the student in seats `b1, b2, ..., bn`.

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

Given a random sequence `{a}` with `n` distinct elements. 

You can swap two adjacent elements each time, and the cost is the smaller one.

You want to know the minimum cost to sort the sequence to a ascending sequence.

For example, if you swap `(ai, ai + 1)`, then the cost of this operation is `min(ai, ai + 1)`.


### Input

The first line of the input contains one integer `n`.

The second line contains `n` integers `a1, a2, ..., an`.

For all cases, `1 <= n <= 1e5, 0 <= ai <= 1e9`.


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

YYJ has many magic beads with two colors, red and blue. If a red bead is on the left of a blue bead and they are next to each other, they will disappear and release `1` unit of magic power. 

YYJ has n strings of beads, each string is consists of `ai` blue beads on the left and `bi` red beads on the right (to make sure they will disappear). Note that `ai` and `bi` can be zero.

YYJ now wants to connect these strings in some order and she is wondering how many units of magic power she can get at most.

### Input

The first line contains an integer `T`, indicating the number of test cases. For each test case:

The first line contains an integer `n (1 <= n <= 100,000)`, indicating the number of string of beads.

Each of the next `n` lines contains two integers `ai, bi (1 <= ai + bi <= 10,000)`.

It is guaranteed that ``∑(ai + bi) <= 500,000``.

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

We use `R` to denote red beads and `B` to denote blue beads.

We have two strings: `BRR`, `BBR` at first.

The string after connection is: `BRRBBR`, which can gain `2` units of magic power.

