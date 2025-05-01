# lab 9

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Travel

### Description

There are $n$ cities and $m$ roads, and each road has a length. These $n$ cities are numbered from $1$ to $n$. Please find the shortest path from city $1$ to city $n$.

### Input

The first line contains two integers $n, m \ (1 \leq n,m \leq 10^5)$, indicating that there are $n$ cities, $m$ roads.

The next $m$ lines, each line contains $3$ integers, $u, v, w \ (1 \leq u,v \leq n, \ 1 \leq w \leq 10^9)$, indicating that there is a unidirectional road from $u$ to $v$, cost $w$.

### Output

Please output the minimum cost from city $1$ to city $n$ in one line. If there is not exist a road from city $1$ to city $n$, please output `-1`.

### Sample Input
```
3 3
1 2 2
2 3 1
1 3 1
```

### Sample Output
```
1
```

<br>

## B. Sign in Problem

### Description

Given a connected undirected graph of $n$ vertices with $m$ edges (may with negative weights) (edge $i$ connects vertices $u_i,v_i$ and with weight $w_i$), you can perform edge deletion, where the gain from deleting an edge is the edge weight of that edge, but you need to ensure that the graph remains connected after performing the edge deletion operation.

Your goal is to **maximize the sum of the gains** from all operations.

### Input

- $1 \leq n \leq 2\cdot 10^5$

- $n−1 \leq m \leq 2\cdot 10^5$

- $1 \leq u_i,v_i \leq n$

- $−10^9 \leq w_i \leq 10^9$

### Output

Print one number as your answer.

### Sample Input
```
3 3
1 2 1
2 3 0
3 1 -1
```

### Sample Output
```
1
```

<br>

## C. Game

### Description

Yuki is an ambitious girl and she is addicted to games.

In one game called The Queen's Gambit, Yuki is controlling the Queen to move in a grid of $n$ rows and $m$ columns, where rows are numbered from $1$ to $n$ and columns are numbered from $1$ to $m$. The cell at the $i$-th row and the $j$-th column is denoted by $(i,j)$. Each cell in the grid contains a point coefficient, denoted by $C_{ij}$.

At first, Yuki can place the Queen on the grid arbitrarily, that is any cells in the grid can be the initial position for the Queen. Every turn Yuki can move the Queen between the cells sharing a common edge. For example, when the Queen is at $(i,j)$, it can be chosen to move to $(i−1,j), \ (i+1,j), \ (i,j−1)$ or $(i,j+1)$, if the destination is not out of the boundary.

Now every time when the Queen is moved from one cell to an **unvisited** cell, Yuki will gain the points which are equal to the product of two point coefficients. It means that Yuki will get $C_{xy} \cdot C_{ij}$ points when the Queen moves from $(i,j)$ to $(x,y)$ and visits $(x,y)$ at the **first** time.

Yuki can stop the game at any time, and she wonders the **maximum** points she can gain.

### Input

The first line contains two integers: $n, m \ (1 \leq n,m \leq 50,000, \ 1 \leq n\cdot m \leq 50,000)$ — rows and columns of the grid.

Each of the next $n$ lines contains $m$ integers. The $j$-th number in the $i$-th line denotes $C_{ij} \ (0 \leq C_{ij} \leq 5000)$.

### Output

Print one line with the result — the maximum points.

### Sample Input
```
1 4
1 2 3 3
```

### Sample Output
```
17
```

<br>

## D. Naive Problem

### Description

Tenshi is given a connected undirected graph with $n$ vertices $m$ edges (weight of every edge is $1$) by Justin_CaO, and a total of $k$ colors of stones, and **exactly one** stone (of one of $k$ colors) at each vertex, the cost of transporting a stone from $u$ to $v$ is the shortest path length from $u$ to $v$. 

Tenshi is busy with ICPC (International Carrot Placing Contest, prepared by FluffyBunny), so this graph is sent to you!

Your goal is to compute the **minimum** cost of transporting a stone of at least $c$ colors to $T$ for each vertex $T \ (T=1,2,...,n)$.

### Input

$1 \leq n \leq 10^5, \ 0 \leq m \leq 10^5, \ 1 \leq c \leq k \leq \min(n,100)$

$4$ integers $n,m,k,c$ in the first line of input.

Next line there are $n$ integers $a_1,a_2,...,a_n$, where $a_i$ is the color of $i$−th vertex's stone.

In the next $m$ lines edges described, edge is described by two integers $u,v$.

### Output

Print $n$ numbers.

$T$-th of them is the minimum cost of vertex $T$.

Separate numbers with spaces.

### Sample Input
```
10 10 3 2
2 2 2 3 2 3 2 2 1 2
9 7
6 5
2 3
2 5
6 4
1 10
9 10
8 5
5 1
9 6
```

### Sample Output
```
2 2 3 2 1 1 1 2 1 1
```

<br>

## E. Simple Problem

### Description

Given a directed graph of $n$ vertices with $m$ edges and a vertex $S$, Tenshi wants you to find the minimum number of directed edges that can be added so that $S$ can reach all vertices on the graph.

### Input

The first line of input consists of three integers $n,m,S$. Here, $1 \leq n \leq 50,000, \ 0 \leq m \leq 50,000, \ 1 \leq S \leq n$.

The following $m$ lines contain directed edges: edge is given as a pair of vertices $u_i,v_i$.

### Output

Output one integer as your answer.

### Sample Input
```
5000 2 238
3212 238
238 3212
```

### Sample Output
```
4998
```

<br>

## F. Portal

### Description

Yuki is a magical girl and she has the ability to activate portals.

The country Yuki lives in has $n$ cities and $m$ roads at certain distances. The cities are numbered from $1$ to $n$ and all the roads are **unidirectional**, that is a road from $u$ to $v$ **cannot** be passed from $v$ to $u$. Also, there are $p$ portals in the country, each of them connects two cities **unidirectional** with **no distance**. Since Yuki doesn't grasp magic thoroughly, she can activate **at most $k$** portals.

Now Yuki is curious about what is the **minimum** distance between $S$ and $T$ if she activates at most $k$ portals.

### Input

The first line contains four integers: $n, m, p$ and $k \ (1 \leq n,m,p \leq 50,000, \ 0 \leq k \leq 10)$ -- the number of cities, roads, portals and the number of portals Yuki can activate at most.

Each of the next $m$ lines contains three integers: $u, v$ and $w \ (1 \leq u, v \leq n, \ 1 \leq w \leq 10^6)$, meaning that there is a unidirectional road from city $u$ to city $v$ at distance $w$.

Each of the next $p$ lines contains two integer: $u$ and $v \ (1 \leq u, v \leq n)$, meaning that there is an inactive portal from city $u$ to city $v$. Please note that when it is **active**, Yuki can **only be** teleported from city $u$ to $v$ unidirectionally.

The last line contains two integers: $S$ and $T \ (1 \leq S,T \leq n)$ -- the origin and destination.

### Output

Print one line with the result -- the minimum distance between city $S$ and $T$.

It is guaranteed that Yuki can move from city $S$ to $T$ by activating at most $k$ portals.

### Sample Input
```
5 6 3 1
1 3 4
1 2 2
3 5 6
2 4 3
3 4 1
4 5 2
2 3
1 4
1 2
1 5
```

### Sample Output
```
2
```
