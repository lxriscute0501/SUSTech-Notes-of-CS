# Lab4 A. Optimized Photography Route in Campus

## Description


There are $n$ scenic spots on campus of SUSTECH and $m$ bidirectional paths connecting them. It is guaranteed that all $n$ scenic spots are connected.

Xiaoqi, a passionate photographer, wants to take $n+m$ consecutive days of photography walks across campus. Each day, he starts at **scenic spot** $1$ and ends at **scenic spot** $n$. His schedule follows these constraints:

* On the $i$-th day ($1 \leq i \leq n$), he must take a photo at **scenic spot** $i$.
* On the $n+j$-th day ($1 \leq j \leq m$), he must take a photo on the $j$-th path.

Since these daily walks are exhausting, Xiaoqi asks for your help in planning his routes to minimize the total walking distance over all $n+m$ days.

## Input Format

* The first line contains two integers: $n$ (number of scenic spots) and $m$ (number of paths).
* The next $m$ lines each contain three integers $u_j, v_j, w_j$, representing a bidirectional path between scenic spots $u_j$ and $v_j$ with a distance of $w_j$.

## Output Format

* Output $n+m$ lines.
* The $k$-th($1 \leq k \leq n+m$) line should contain a single integer: the **shortest walking distance from scenic spot** $1$ to scenic spot $n$ while fulfilling the $k$-th day's requirement.

## Example 1

### Input

```
4 4
1 2 3
2 3 4
3 4 5
1 3 5
```

### Output

```
10
12
10
10
12
12
10
10
```

## Example 2

### Input

```
6 8 
1 2 3 
2 3 4
1 4 5
2 5 8
5 6 2
3 5 7 
3 6 1
4 5 6
```

### Output

```
8
8
8
13
12
8
8
8
13
13
12
16
8
13
```

## Limitations

For 10\% of cases: $n \leq 10, m \leq 20$.\
For 30\% of cases: $n \leq 100, m \leq 200$.\
For 60\% of cases: $n \leq 1000, m \leq 2000$.\
For 100\% of cases: $n \leq 100000, m \leq 200000$.

It is guaranteed that $1 \leq u_j, v_j \leq n$ and $1 \leq w_j \leq 10000$.

<br>

# Lab4 B. Optimized Delivery Network for Campus Express Stations

## Description

The campus of SUSTECH has $n$ express stations, labeled $1$ to $n$, where $1$ serves as the **Central Hub**. There are $m$ existing **one-way delivery roads**, each consuming $w_i$ kWh per month.  It is guaranteed that all stations are reachable from the **Central Hub**.
The university president has identified inefficiencies in the **campus delivery roads**, resulting in high electric vehicle maintenance costs.
He has tasked you with optimizing the delivery network under the **following constraints**:

1. **Retain exactly $n-1$ roads**.
2. **Ensure the shortest delivery time from the Central Hub to each station remains unchanged**.
3. **Minimize total monthly electricity consumption**.

## Input Format

* The first line contains two integers: $n$ (number of stations) and $m$ (number of existing roads).
* The next $m$ lines each contain three integers: $( u_i, v_i, w_i )$, representing a one-way road from station $u_i$ to $v_i$ with a monthly energy cost of $w_i$ kWh.

**Note: There may be self-loops or multiple edges.**

## **Output Format**

* A single integer: the **minimum total monthly electricity consumption** after optimization.

## Example 1

### Input

```
3 5
1 3 2
2 3 3
1 3 2
3 2 2
1 2 4
```

### Output

```
4
```

## Example 2

### Input

```
5 10
1 2 5
2 3 6
1 4 4
3 5 4
3 4 5
1 5 5
1 2 5
2 3 6
3 4 6
4 5 4
```

### Output

```
20
```

## Limitations

For 20\% cases, $n \leq 10$ , $m \leq 20$

For 60\% cases, $n \leq 10^3$ , $m \leq 5 \times 10^4$

For 100\% cases, $n \leq 5 \times 10^4$ , $m \leq 5 \times 10^5$

It is guaranteed that $1 \leq u_i, v_i \leq n$ and $1 \leq w_i \leq 10^9$.

