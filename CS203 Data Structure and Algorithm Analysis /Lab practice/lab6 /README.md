# lab 6

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. K-ary tree

### Description

Given a complete $K$-ary tree with $N$ nodes, please find the number of leaf nodes of this tree.

A $K$-ary tree is a rooted tree where every internal node has at most $K$ child nodes, a $K$-ary tree of height $h$ is complete if:

- Level $0, 1, ..., h-1$ are all full
- At level $h$, the leaf nodes are as far left as possible.

### Input

The first line will be an integer $T \ (1 \leq T \leq 1000)$, which is the number of test cases.

For each test data: \
The first line contains two integer $N \ (2 \leq N \leq 10^8)$ and $K \ (2\leq K \leq 100)$ — the number of nodes, and the number child nodes it has at most.

### Output

For each test cases, print one line with one integer, the number of leaf nodes of this tree.

### Sample Input
```
2
5 2
10 3
```

### Sample Output
```
3
7
```

<br>

## B. Level traversal

### Description

There is an $m$-ary tree. You should find the level traversal order of this tree. The root node of the tree is $1$. When you traverse child nodes with the same parent node, the traversal order of the child nodes should be in **ascending order**.

### Input

In the first line, $T$ are the number of the test ($1\leq T\leq 20$).

In each case:\
The first line have one integer $N$. $N$ are the number of the nodes. ($1 \leq N \leq 10^5$).\
The next line have $N-1$ integers $a_1, a_2, ..., a_{n-1}$, which $a_i$ is the father of the $i+1$. ($1 \leq a_i \leq N$).

### Output

Output is the order of the level traversal. When you traverse child nodes with the same parent node, the traversal order of the child nodes should be in **ascending order**.

### Sample Input
```
2
4
1 1 1
5
1 1 3 2
```

### Sample Output
```
1 2 3 4
1 2 3 5 4
```

<br>

## C. Paths

### Description

Given a rooted tree numbered from $1$ to $n$, each edge has a weight $w$. The root node of the tree is node $1$. You are asked to calculate the number of paths that start from the root, terminate in a leaf node, and satisfy the sum of edge weights in the path equals to $num$.

### Input

The first line contains two integers $n$ and $num \ (1 \leq n \leq 5\cdot 10^6, 1 \leq num \leq 2\cdot 10^9)$, indicating the number of tree nodes and the target number.

Then $n−1$ lines follow. Each line contains three integers $u,v,w \ (1 \leq u,v \leq n, \ 1 \leq w \leq 100)$ describing an edge. The first two integers are the indices of nodes that form an edge and the last integer indicates the weight of the edge.

### Output

Output an integer which means how many paths satisfying the sum of edge weights in the path equals to $num$.

### Sample Input
```
6 6
1 2 2
1 3 3
3 4 4
3 5 3
2 6 4
```

### Sample Output
```
2
```

<br>

## D. Cut the stick

### Description

Lanran wants to cut one stick with length $L$ into exactly $N$ sticks with length $L_i \ (i = 1, 2, ..., N)$, so $L = \sum L_i$. However, the cost to cut one stick in to two pieces is the length of the stick, that means cutting a stick with length $x$ will cost $x$. Now he wants to know the minimal cost if he cuts stick optimally to get $N$ sticks.

### Input

The first line will be an integer $T \ (1 \leq T \leq 100)$, which is the number of test cases.

For each test data: \
The first line contains an integer $N \ (1 \leq N \leq 1000)$ — the number of sticks Lanran needs to get.\
Then the next one line contains $N$ integers, the $i$-th integer $L_i \ (1 \leq L_i \leq 10^5)$ indicates the length of $N$ sticks Lanran wants to get.

### Output

For each case, contains one line, print the minimal minimal cost.

### Sample Input
```
1
4
1 4 2 6
```

### Sample Output
```
23
```

### Hint

**Do not use any heap and BST related STL!!**

<br>

## E. Giants

### Description

There are $n$ cities numbered from $1$ to $n$, and $n−1$ roads connecting these $n$ cities, i.e., it is a tree with $n$ nodes. Each road takes one day for giants to travel through. City $1$ is a huge city and can host all giants. However, other cities are small. Each small city can only host one giant at the same time. Initially, some giants are in small cities. They set out at the same time and want to gather in city $1$. In each day, every giant can choose to go to another city or stay at the current city. Please find the minimum needed days for all giants to gather in city $1$.

### Input

The first line contains an integer $n \ (2 \leq n \leq 400,000)$ which means the number of cities.

Then $n−1$ lines follow. Each line contains two integers $u,v \ (1 \leq u,v \leq n)$ which means a road between city $u$ and city $v$.

Then one line contains an integer $m \ (1 \leq m \leq n−1)$ which means the number of giants.

Then one line contains $m$ integers which describe the indices of the cities where these giants are in initially. Different giant is in a different small city initially.

### Output

Print the minimum number of days needed for all giants to gather in city $1$.

### Sample Input
```
5
1 2
1 3
3 4
3 5
4
2 3 4 5
```

### Sample Output
```
3
```

### Hint

**Explanation for the example:** In day $1$, two giants in city $2$ and $3$ can go to city $1$. However, two giants in city $4$ and $5$ cannot both go to city $3$ since city $3$ is small. One of the two has to stay at the current city in day $1$, and arrives in city $1$ at the end of day $3$. Therefore, the answer is $3$.

<br>

## F. Node Activation

### Description

Given a tree whose nodes are numbered from $1$ to $n$. Each node has a predefined value $p_i$. The task is to assign a non-negative value $e_i$ (i.e., $e_i \geq 0$) for each node to activate all nodes.

A node $i$ is activated if and only if: \
(1) there exists two different nodes $j$ and $k$, and node $i$ is on the path between nodes $j$ and $k$ ($j \neq k$, but node $i$ can be node $j$ or node $k$); \
(2) $\min(e_j,e_k) \geq p_i$.

Please find the minimum value of $(e_1 + e_2 +...+ e_n)$ to activate all nodes.

### Input

The first line contains an integer $n \ (2 \leq n \leq 200,000)$ which means the number of nodes.

Then $n−1$ lines follow. Each line contains two integers $u,v \ (1 \leq u,v \leq n)$ which means an edge between node $u$ and node $v$.

Then one line contains $n$ integers $p_i \ (1 \leq p_i \leq 10^8)$.

### Output

Print the minimum value of $(e_1 + e_2 +...+ e_n)$ to activate all nodes.

### Sample Input
```
4
1 2
2 3
2 4
2 3 1 1
```

### Sample Output
```
7
```

### Hint

**Explanation for the example:** one optimal assignment is $e_1=3, \ e_2=0, \ e_3=3, \ e_4=1$. Node $1$ is activated by nodes $1$ and $3$. Node $2$ is activated by nodes $1$ and $3$. Node $3$ is activated by nodes $3$ and $4$. Node $4$ is activated by nodes $4$ and $1$.

**Tips:** You can choose the node with the maximum $p_i$ value as the root.