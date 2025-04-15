# Lab5 A. Hide and Seek

## Description

Xiaosan and Xiaowu are part of the campus security duo, and they’re on a mission to catch the mischievous student Xiaoqi. But Xiaoqi isn’t interested in being caught — he just wants to find a quiet place to sleep.

The SUSTech campus is a magical network of **N teleportation portals** forming a **tree**. The travel time between portals $x$ and $y$, denoted by $d(x, y)$, is the number of edges on the **shortest path** connecting them.

Xiaoqi has discovered a clever invisibility trick: if he stands at a portal $z$ such that $d(x, z) = d(y, z)$ — where Xiaosan is at portal $x$ and Xiaowu is at portal $y$ — then neither of them can spot him. He's perfectly hidden!

Xiaoqi has received $Q$ detection plans from Xiaosan and Xiaowu, each represented as a pair of portals $(x_i, y_i)$, where Xiaosan is at portal $x_i$ and Xiaowu is at portal $y_i$. However, Xiaoqi is too sleepy to analyze them all. Can you help him determine, for each pair, how many portals z exist where he can hide undetected?

## **Input Format**

* The first line contains an integer $N$.

* For the following $N-1$ lines, each line contains two integers $u, v$, representing an undirected edge $(u, v)$.

* The next line contains an integer $Q$.

* For the following $Q$ lines, the $i-th$ line contains two integers $x_i, y_i$, denoting a query such that Xiaosan is on $x_i$ and Xiaowu is on $y_i$.

## **Output Format**

Output $Q$ lines. For each query, output a single integer denoting the answer.

## **Example**

### **Input**

```
5
1 2
1 3
3 4
3 5
3
4 5
1 2
3 3
```

### **Output**

```
3
0
5
```

## Limitations & Hints

For 50\% test cases:

* $N, Q \le 1000$

For 100\% test cases:

* $N, Q \le 10^5$
* $1 \le x_i, y_i \le N$.
* A valid tree is guaranteed to be given.

<br>

# Lab5 B. Package Delivery Optimization

## Problem Description

In the X Building's delivery room at SUSTech, there are $n$ shelves arranged in a straight line. The position of the i-th shelf is given by coordinate $x_i$, where $0 ≤ i ≤ n−1$.

A batch of packages needs to be delivered to these shelves. The i-th shelf requires $m_i$ packages. The total number of packages is $\Sigma m_i$.

The delivery task is assigned to Xiaoqi. Xiaoqi can carry at most $k$ packages at a time. Initially, Xiaoqi and all the packages are located at the origin (coordinate $0$). Each time, Xiaoqi picks up a number of packages from the origin, delivers them to the required shelves, and then returns to the origin to carry the next batch.

Please calculate the **minimum total distance** Xiaoqi needs to travel to deliver all packages to their destinations. After all packages are delivered, Xiaoqi does **not** need to return to the origin.

## Input Format

* The first line contains two integers $n$ and $k$, where $n$ is the number of shelves, and $k$ is the maximum number of packages Xiaoqi can carry at once.

* The second line contains `n` integers, the coordinates of the shelves: $x_0, x_1, ..., x_{n−1}$.

* The third line contains $n$ integers, the number of packages required at each shelf: $m_0, m_1, ..., m_{n−1}$.

## Output Format

Output a single integer — the minimum total distance Xiaoqi needs to travel to deliver all packages.

## Example 1

### Input

```
5 1
1 3 2 4 5
1 1 1 1 1
```

### Output

```
25
```

### Explanation

Since Xiaoqi can only carry 1 package at a time, each delivery requires a round trip from the origin. The optimal delivery route is:

`0 → 1 → 0 → 2 → 0 → 3 → 0 → 4 → 5`

Total distance is:

`1 + 1 + 2 + 2 + 3 + 3 + 4 + 4 + 5 = 25`.

***

## Example 2

### Input

```
5 3
-7 -9 -8 7 8
1 1 1 1 2
```

### Output

```
25
```

### Explanation

Xiaoqi can carry up to 3 packages at once. The optimal delivery route is:

`0 → 7 → 8 → 0 → -7 → -8 → -9`

Total distance is:

`7 + 1 + 8 + 7 + 1 + 1 = 25`.

## Limitations

For 40\% cases，
$n\le 100, k\le 10$

For 60\% cases，
$n\le 10^4, k\le 10^5$

For 100\% cases，
$n\le 10^5, k\le 10^5$

It is guaranteed that $1 \leq m_i \leq 10$ and $-10^9 \leq x_i \leq 10^9$.

