# lab 8

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Adjacency Matrix

### Description

Given a directed graph $G$ with $n$ nodes and $m$ edges. Please print the adjacency matrix $A$ of $G$.

*Hints:* Adjacency matrix is a method to represent a graph. Suppose we have a directed graph $G$, if there is an edge from node $i$ to node $j$ in $G$, then we have $A[i][j] = 1$ in corresponding adjacency matrix $A$, otherwise, $A[i][j] = 0$.

### Input

The first line will be an integer $T \ (1 \leq T \leq 50)$. $T$ is the number of test cases.

For each test case, the first line will be two integers $n$ and $m$. ($1 \leq n \leq 500, \ 0 \leq m \leq n^2$)

Then there will be $m$ lines. Each line will have two integers $x, y$. $x, y$ means there is an edge from $x$ to $y$. All nodes are labeled from $1$ to $n$. 

### Output

For each test case, print the adjacency matrix.

### Sample Input
```
2
3 5
1 2
2 1
1 3
3 2
2 3
1 0
```

### Sample Output
```
0 1 1
1 0 1
0 1 0
0
```

<br>

## B. Shortest path

### Description

Alice has a graph of $n$ nodes and $m$ undirected edges. These nodes are numbered from $1$ to $n$, and now she wants to know the shortest path from node $s$ to other nodes. If there is no path between $s$ and node $i$, output `-1`. ($1 \leq i \leq n$).

### Input

The first line will be an integer $T$, which is the number of the test cases ($1 \leq T \leq 10$). 

For each test case, the first line will be three integers, $n \ (1 \leq n \leq 10^5)$, $m \ (1 \leq m \leq 5\cdot 10^5)$ and $s \ (1 \leq s \leq n)$. \
The following are $m$ lines, and each line will be two integers $x$ and $y$, which means there is an undirected edge between $x$ and $y$.

### Output

For each test, output numbers in one line. The $i$-th number means the shortest path between $s$ and node $i$. If the path does not exist, then print `-1` instead.

### Sample Input
```
1
4 4 1
1 2
2 3
3 4
1 4
```

### Sample Output
```
0 1 2 1
```

<br>

## C. Defensive Tower

### Description

On the mainland, there is a fire-breathing dragon called "lanran", who is always burning cities and attacking people. So Pisces decides to build some defensive towers in the kingdom to protect his people. A defensive tower is able to protect the city where it is located and the cities adjacent to it. However, building a tower costs a lot, so Pisces could only build at most $⌊\frac{n}{2}⌋$ defensive towers ($n$ is the total number of cities in the kingdom). Please find a way to build defensive towers in order to protect the whole kingdom. If there are multiple answers, print any.

By saying that "two cities are adjacent", it means that there is one undirected road connecting them.

### Input

The first line contains a single integer $t \ (1 \leq t \leq 2\cdot 10^5)$, which represents the number of test cases. Then $t$ test cases follow.

In each test case, the first line contains $2$ integers $n \ (2 \leq n \leq 2\cdot 10^5)$ and $m \ (n−1 \leq m \leq \min(2 \cdot 10^5, \frac{n \cdot (n−1)}{2}))$, indicating the number of cities and the number of roads. And in each of the next $m$ lines, there are $2$ integers $u$ and $v \  (1 \leq u,v \leq n)$ representing the indexes of the cities that this road connects.

There are no self-loops or multiple edges in the given graph. It is guaranteed that the given graph is connected and $\sum m \leq 2 \cdot 10^5$.

### Output

For each test case print two lines.

In the first line print $k$ — the number of chosen cities.

In the second line print $k$ distinct integers $c_1,c_2,...,c_k$ in any order, where $c_i$ is the index of the $i$-th chosen city.

It is guaranteed that the answer exists. If there are multiple answers, you can print any.

### Sample Input
```
2
4 6
1 2
1 3
1 4
2 3
2 4
3 4
6 8
2 5
5 4
4 3
4 1
1 3
2 3
2 6
5 6
```

### Sample Output
```
2
1 3
3
4 3 6
```

<br>

## D. Ancestor

### Description

Ella has a tree of $n$ nodes numbered from $1$ to $n$. There are $m$ queries, and each query has two integers $x$ and $y$ to ask whether $y$ is an ancestor of $x$ or not, and $x$ is an ancestor of $x$. However, Ella does not know how to deal with it. Please find a solution.

### Input

The first line will be an integer $T$, which is the number of the test cases($1 \leq T \leq 10$). 

For each test case, the first line will be two integers $n \ (1 \leq n \leq 1.5\cdot 10^5)$ and $m \ (1 \leq m \leq 1.5\cdot 10^5)$.

The following are $n - 1$ lines, and each line will be two integers $x$ and $y$, which means that $y$ is the father of $x$. 

The following are $m$ lines, and each line will be two integers $x$ and $y$, which means a query $(x, y)$. If $y$ is an ancestor of $x$, output `Yes`, else output `No`.

### Output

For each test output $m$ lines to answer the queries.

### Sample Input
```
1
2 1
2 1
1 2
```

### Sample Output
```
No
```

<br>

## E. Sum

### Description

An $N \times M$ matrix of non-negative integers, please take out serveral numbers from it such that the summation of these numbers is maximized. You should guarantee that any two taken out numbers are not adjacent, we say $a$ is adjacent to $b$ if $b$ is one of $a$'s $8$-connected neighborhoods in the matrix.

### Input

The first row has a positive integer $T \ (1 \leq T \leq 20)$, which indicates that there are $T$ groups of data.

For each set of data, the first row has two positive integers $N,M \ (1 \leq N,M \leq 6)$ , which describe the number matrix as $N$ rows and $M$ columns.

The next $N$ rows, with $M$ non-negative integers in each row, describe this numerical matrix.

### Output

A total of $T$ rows, one non-negative integer per row, and the output of the resulting answer.

### Sample Input
```
3
4 4
67 75 63 10
29 29 92 14
21 68 71 56
8 67 91 25
2 3
87 70 85
10 3 17
3 3
1 1 1
1 99 1
1 1 1
```

### Sample Output
```
271
172
99
```

<br>

## F. The Elves

### Description

To make the kingdom more prosperous, Pisces decides to ally with the elves living in the forest. However, the elven elders want to test Pisces, so they give him a simple question. Given a DAG with $n$ nodes and $m$ edges, the elven elders want to know the value of
$$
\sum_{i=1}^n \sum_{j=1}^n count(i,j) \cdot a_i \cdot b_j \mod (10^9+7),
$$

where $count(x,y)$ is defined by the number of different paths from $x$ to $y$ and $count(x,x) = 0$, and $a, b$ are two given arrays. It is too hard for Pisces to answer this question, so he turns to you for help.

### Input

The first line contains an integer $T \ (1 \leq T \leq 10)$, which denotes the number of test cases.

For each test case, the first line contains $2$ integers $n$ and $m \ (1 \leq n,m \leq 10^5)$ — the number of nodes and the number of edges, respectively. Each of the next $n$ lines contains $2$ integers $a_i$ and $b_i$. And for the next $m$ lines, each line contains $2$ integers $u$ and $v$ denoting a directed edge going from node $u$ to node $v \  (1 \leq u,v \leq n)$.

### Output

For each test case, print the answer.

### Sample Input
```
3
3 3
1 1
1 1
1 1
1 2
1 3
2 3
2 2
1 0
0 2
1 2
1 2
2 1
500000000 0
0 500000000
1 2
```

### Sample Output
```
4
4
250000014
```
