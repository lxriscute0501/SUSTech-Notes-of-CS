# Lab6 A. Inversion Pair

## Description

At the Southern University of Science and Technology, students receive their grade report at the end of each semester, listing the scores for all their courses.

One day, while studying in the library, Xiaoming and Xiaohong were discussing an interesting and slightly frustrating idea: what if the order in which they planned their revision doesn't align with their actual performance?

They decided to measure this misalignment using a concept called an "i**nversion pair**":

For a list of scores, if there exists a pair of courses 
$i<j$, but the score for course $i$ is **greater** than the score for course $j$, i.e., $a_i > a_j$, then $i, j$ is considered an inversion pair.

Xiaoming wants to count how many inversion pairs are present in his grade list, to evaluate how poorly his study plan matched his results.

Can you help him?

## Input Format

The first line contains an integer $n$, the number of courses.

The second line contains $n$ integers, representing the scores of each course in the **planned revision order**.

## Output Format

Output a single integer: the number of inversion pairs in the score list.

## Sample Input
```
5
7 4 2 5 3
```

## Sample Output
```
7
```

<br>

# Lab6 B. Tree Path Sum Queries

## Description

Given a tree where each edge has a weight, and a target path sum $k$, your task is to find the number of **paths** in the tree whose total weight equals $k$. A path is defined as a sequence of nodes connected through edges, and the path sum is the sum of the weights of the edges along that path.

## Input Format:

* The first line contains two integers $n$ and $k$, representing the number of nodes and the target path sum.

* The next $n−1$ lines each contain three integers $u, v, $ and $w,$ indicating that there is an edge between node $u$ and node $v $ with weight $w$.

  Where:

  * $1 \leq n \leq 10000$: the number of nodes in the tree.
  * $0 \leq k \leq 1000000$: the target path sum.
  * $1 \leq u, v \leq n$: the node indices.
  * $1 \leq w \leq 10000$: the weight of each edge.

## Output Format:

* Output "Yes" if there exists paths in the tree whose total weight equals $k$, otherwise output "No".

## Sample Input:

```
5 7
1 2 2
1 3 1
2 4 3
2 5 4
```

## Sample Output:

```
Yes
```

* We are looking for paths with a sum equal to $7$.
* In this tree, there exists one path ($4 \to 2 \to 5$) with a total weight of $7$.


## Note:

* A tree is an acyclic connected graph, and there exists a unique path between any two nodes.
