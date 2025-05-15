# Lab9 A. Frog

## Problem Description

There is a river of width $\rm L$, and there are $n$ stones located at positions $a_1, a_2, ..., a_n$. A frog wants to jump from one side of the river (position $0$) to the other side (position $\rm L$) under the following rules:

* The frog can only land on a stone, or on the riverbanks at position $0$ and position $\rm L$.
* The frog can only jump forward, not backward.
* The frog has two types of jumps:
  * It can jump exactly $m_a$ meters, up to $x_a$ times, each costing $c_a$ stamina.
  * It can jump exactly $m_b$ meters, up to $x_b$ times, each costing $c_b$ stamina.

Your task is to determine the **minimum total stamina** required for the frog to reach position $\rm L$ starting from position $0$.

If it is **impossible** to reach the opposite riverbank, output `-1`.

## Input Format

* The first line contains two integers: $n$ and $\rm L$
* The second line contains three integers: $m_a, x_a, c_a$
* The third line contains three integers: $m_b, x_b, c_b$
* The fourth line contains $n$ integers: $a_1, a_2, ..., a_n$

## Output Format

Output a single integer: the minimum stamina required to reach the other side. If it is impossible, output `-1`.

## Sample Input #1

```
10 20
2 9 9
3 6 5
3 4 5 7 9 11 13 15 17 19
```

## Sample Output #1

```
73
```

## Sample Input #2

```
15 50
5 20 9
7 15 5
3 4 5 7 12 17 18 19 21 26 31 37 38 41 45
```

## Sample Output #2

```
52
```

## Constraints

For 20% cases: $m,n,x,\rm L,c\le 50$

For 50% cases: $c\le 100,n,x,m\le 200, \rm L\le 10^4$

For 100% cases: $c\le 100,m\le500,x\le 5\times 10^3, n\le 10^5, \rm L\le10^6$

<br>

# Lab9 B. Milk Candy

## Problem Background

At the end of the semester, a student at SUSTech wants to thank a TA who has provided significant help throughout the term.    To show appreciation, the student decides to buy the TA’s favorite treat — milk candy.    However, after recently buying gifts for friends during his holiday, the student only has **n yuan** left.    Fortunately, the nearby campus store offers **m types** of milk candy, each sold as a whole package and not individually.

The student wants to buy some candy without exceeding the budget and aims to **maximize the total sugar content** obtained.    Please help calculate the maximum sugar he can get, along with the specific number of packages to buy for each candy type.

Given an integer **n** (the total amount of money available) and **m** (the number of candy types), along with the **price and sugar content** of each type of candy, determine:

* The **maximum total sugar** the student can obtain within the budget;
* A valid **purchase plan**, showing how many units of each candy to buy.

Each candy type has an **unlimited supply**, and **each unit has a positive integer price**.

It is guaranteed that **there exists exactly one optimal solution**.

## Input Format

* The first line contains two integers, **n** and **m** — the amount of money available and the number of candy types.
* The next **m lines** each contain two integers **a** and **b**, where **a** is the price and **b** is the sugar content of that type.

## Output Format

* The first line outputs a single integer: the **maximum total sugar** that can be obtained.
* The following **m lines**: the **j-th line** (1-based) should output the number of units bought of the **j-th type** of candy.

## Sample Input #1

```
70 3
71 100
69 1
1 2
```

## Sample Output #1

```
140
0
0
70
```

## Constraints

* For 20% of the test cases: $0 < n \leq 100$, $0 < m \leq 20$
* For 60% of the test cases: $0 < n \leq 1000$, $0 < m \leq 4000$
* For 100% of the test cases: $0 < n \leq 40000$, $0 < m \leq 4000$
* All input values are positive integers within the **int** range
* All prices satisfy **$a \geq 1$**.

## Notes

* Each candy type can be purchased **in unlimited quantity**.
* There is **exactly one** optimal solution.
