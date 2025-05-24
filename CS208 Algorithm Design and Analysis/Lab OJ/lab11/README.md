# Lab11 A. Network Maximum Flow

## Problem Description

Given a directed network graph with a source and a sink, your task is to calculate the **maximum flow** from the source to the sink.

## Input Format

The first line contains four positive integers $n, m, s, t$ — the number of nodes, the number of directed edges, the source node index, and the sink node index, respectively.

Each of the next $m$ lines contains three positive integers $u_i, v_i, w_i$, indicating that there is a directed edge from node $u_i$ to node $v_i$ with capacity $w_i$ (i.e., the maximum possible flow through this edge is $w_i$).

## Output Format

Output a single positive integer — the **maximum flow** from the source to the sink.

## Sample Input

```
4 5 4 3
4 2 30
4 3 20
2 3 20
2 1 30
1 3 30
```

## Sample Output

```
50
```

## Explanation for the Sample

There are 3 valid flow paths:

* $4 \to 2 \to 3$: can carry **20** units of flow.
* $4 \to 3$: can carry **20** units of flow.
* $4 \to 2 \to 1 \to 3$: can carry **10** units of flow (note that edge $4 \to 2$ has already used 20 units of its 30-unit capacity).

So the total flow is $20 + 20 + 10 = 50$.

## Constraints

* For **30%** of the data: $n \leq 10$, $m \leq 25$.
* For **100%** of the data: $1 \leq n \leq 200$, $1 \leq m \leq 5000$, $0 \leq w < 2^{31}$.

<br>

# Lab11 B. Xiaofeng's International Communication Network

## Problem Description

Xiaofeng stays in touch with his friends around the world through a computer network. If there's a path of connected devices (computers) between two people, they can exchange messages.

However, sometimes some relay devices (computers) might fail due to accidents or maintenance. Once a device fails, it can no longer be used, and all its connections are cut off as well.

Now, Xiaofeng wants to know: **what is the minimum number of relay devices that need to be disabled to completely block communication between him and one of his friends?**

**Note:**

* All connections in the network are **bidirectional**.
* There is **at most one** connection between any two devices.
* The devices used by Xiaofeng and his friend **cannot be disabled**.
* The goal is to **block communication** between them with the **minimum number of disabled devices**.

## Input Format

The first line contains four integers: $N, M, s, t$

* $N$: total number of devices, labeled from $1$ to $N$
* $M$: number of direct connections
* $s$: the device used by Xiaofeng
* $t$: the device used by his friend

The next $M$ lines each contain two integers $u\ v$, indicating that device $u$ and device $v$ are directly connected.

## Output Format

A single integer: the **minimum number of devices** that need to be disabled (excluding $s$ and $t$) to block all communication between $s$ and $t$.

## Sample Input

```
3 2 1 2
1 3
2 3
```

## Sample Output

```
1
```

## Constraints

* $1 \leq N \leq 100$
* $1 \leq M \leq 600$
