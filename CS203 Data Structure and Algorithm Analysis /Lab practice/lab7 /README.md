# lab 7

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Are you my new friend?

### Description

You are given a complete binary tree and each node has a **distinct** key value. Now you are asked to determine whether the tree is a heap or not. If it is a heap, you are to determine if it is a Max-heap or a Min-heap.

### Input

The first line contains $N \ (2 \leq N \leq 10^6)$, denoting the number of nodes in the tree.

The second line contains $N$ integers $A_1,A_2,...,A_N \ (1 \leq A_i \leq 10^9)$, denoting the key value of node $i$. It is guaranteed that no two key values are the same.

To simplify the problem, the tree is constructed as follows:

-   The root of the tree is node $1$.
-   For each $2 \leq i \leq N$, There is an edge between $i$ and $⌊\frac{i}{2}⌋$.

### Output

If the tree is a Max-heap, print `Max`.

If the tree is a Min-heap, print `Min`.

If the tree is not a heap, print `Neither`.

### Sample Input
```
7
55 6 12 4 2 5 8
```

### Sample Output
```
Max
```

### Hint

**Do not use any heap and BST related STL!!**

<br>

## B. Heap Process [Easy ll]

### Description

Initially, there are $n$ nodes, with index $1, 2, 3, ..., n$. Every nodes has a value $a_i$. They enter the tree in order and make the tree as a Max Heap (same value will not swap). This process can make biggest value node to reach the top. After $n$ nodes enter the tree, David wants to find a node with index $p$. Could you please tell him the node's position with level $x$, and the index $y$ on that level?

### Input

The first line contains an integer $T \ (1 \leq T \leq 10)$, indicating the test cases. In each test case, the first line contains an integer $n \ (1 \leq n \leq 10^5)$. 

The second line contains $n$ integers, $a_1,a_2,a_3,...,a_n \ (1 \leq a_i \leq 10^9)$ indicating the node's value. 

The third line contains an integer, indicating the node’s index.

### Output

For each test case, please print the level $x$ and index in that level $y$ in one line. Note that, the root position is $(1, 1)$.

### Sample Input
```
3
2
2 1
1
3
1 2 3
2
5
3 1 2 2 3
4
```

### Sample Output
```
1 1
2 2
3 2
```

### Hint

For the second test case: Initially, $1$ is enter $(1, 1)$. When $2$ arrive, it is on $(2, 1)$, but it is bigger than $1$, so, $1$ and $2$ swap. Then, $2$ is in $(1, 1)$, $1$ is in $(2, 1)$. When $3$ arrive, it is on $(2, 2)$, but it is bigger than $2$, so, $2$ and $3$ swap. Then, $3$ is in $(1, 1)$, $2$ is in $(2, 2)$.

<br>

## C. Barcelona FC Manager [Middle l]

### Description

Karl is a fan of Barcelona FC (Football Club), but recently he is dissatisfied with the manager. So, he make a daydream to become a Barcelona FC manager and buy some football players. The Barcelona FC has infinite money, that means Karl can buy anyone he want. There are $n$ players. For each player, it has a power $a_i$. For Each hour, he can only buy one player. However, for the $i$-th player, he can only be bought before $b_i$-th hour (including the $b_i$-th hour). Karl is short with algorithm, please help him to buy some players, which can make the sum of power as max.

### Input

The first line contains an integer $T \ (1 \leq T \leq 10)$, indicating the number of test cases.

For each test cases, the first line contains an integer $n \ (1 \leq n \leq 10^5)$. \
The second line contains $n$ integers $a_1,a_2,a_3,...,a_n \ (1 \leq a_i \leq 10^5)$, indicating the power of each player. \
The third line contains $n$ integers $b_1,b_2,b_3,...,b_n \ (1 \leq b_i \leq 10^4)$, and $b_i$ indicating Karl can only buy the player before $b_i$-th hour (including the $b_i$-th hour).

### Output

For each test case, please print the max sum of power in one line.

### Sample Input
```
3
3
2 1 3
1 1 2
4
50 10 20 30
2 1 2 1
7
20 2 10 100 8 5 50
1 1 3 2 2 20 10
```

### Sample Output
```
5
80
185
```

<br>

## D. Stream Processing [Middle II]

### Description

VinceBlack is working on stream processing. There is now an infinite stream of data that can be thought of as producing an integer per second. Your task is to collect each number and then output the current $k$-th largest number (for all the numbers collected) every $100$ seconds. If the currently collected number is less than $k$, the smallest number is output.

The $k$-th largest number indicates that if sorted in descending order, this number will be ranked in the $k$-th position.

### Input

The input contains three integers $t \ (100 \leq t \leq 10^6)$, $k \ (1 \leq k \leq 1000)$ and $s \ (0 \leq s \leq 10^5)$.

$t$ is the number of seconds you need to process.

Define $a(n)=n+sum$ of the digits of $n$, and the number appearing at $i$-th ($i$ starts at $1$) second in the data stream is $a(i+last \ ans)$.

$last \ ans=s$ in the beginning and will be updated after each answer is calculated.

### Output

$⌊\frac{t}{100}⌋$ integers in a line, represents the answers in each $100$ seconds.

### Sample Input
```
1926 8 17 
```

### Sample Output
```
117 319 623 1024 1529 2131 2840 3649 4559 5571 6686 7906 9219 10624 12127 13737 15447 17258 19171 
```

### Hint

More information about $a(n)$: [ref](https://oeis.org/A062028)

<br>

## E. Funny Fluffy Tuzi

### Description

Fluffy Funny Tuzi (FFT) has $N$ piles of carrot lining up in a line, the $i$-th of which has $a_i$ carrots.

Tuzi wants as many carrots as possible. It can magically merge two adjacent piles $a_i,a_{i+1}$ to produce a new pile with $(a_i \oplus a_{i+1})+1$ carrots. Here $\oplus$ means binary xor.

Nevertheless, Tuzi does not want to think optimally. Everytime it will pick the pile with the least carrots, and merge it with one of its adjacent pile(s) so that the new pile has the maximum possible number of carrots.

- If multiple piles contain the least number of carrots, then the left-most such pile is chosen.
- If for the chosen pile there are two merge choices and both choices yield a pile with the same number of carrots, the the left adjacent pile is merged.

Tuzi will keep merging until there is only one pile of carrots. It wonders how many carrots it can eventually obtain.

### Input

The first line contains an integer $N$.

The second line contains $N$ integers $a_1,a_2,...,a_N$.

- $1 \leq N \leq 500,000$
- $0 \leq a_i < 2^{30}$

It is guaranteed that anytime any pile contains less than $2^{31}$ carrots.


### Output

Output a single number: the number of carrots in the final pile.

### Sample Input
```
5
3 6 6 7 1 
```

### Sample Output
```
7
```

### Hint

**Do not use any heap and BST related STL!!**

<br>

## F. Pet Adoption [Hard II]

### Description

Lanran opened a pet adoption center. Each pet has a characteristic value $p \ (0 < p < 2^{31})$ and each adopter also has a value $q \ (0 < q < 2^{31})$.

Lanran needs to provide the following services:

For a pet with characteristic value $p$ arriving, it will be adopted by a person staying in the center whose $q$ is the minimum closest to $p$ or stay in the center if there is no adopter left.
For an adopter with value $q$ arriving, he/she will choose a pet staying in the center whose $p$ is the minimum closest to $q$ or stay in the center if there is no pet left.

$a$ is the minimum closest to $v$ in set $S$ if and only if:

- for all $a_x \in S$, there is $|a−v| \leq |a_x−v|$
- for all $a_i \in S$ and $|a−v|=|a_i−v|$, there is $a \leq a_i$

The dissatisfaction for each adoption is defined as $|p−q|$.

### Input

The first line is a positive integer $n \ (n \leq 80,000)$, which represents the total number of pets and adopters who come to the adoption center. 

The next $n$ lines describe the pets and adopters who came to the adoption center in the order of arrival. Each line has two positive integers $a, b$, where $a=0$ for pets, $a=1$ for adopters, and $b$ for character values.

### Output

A positive integer representing the sum of the dissatisfaction of all adopted adopters of pets.

### Sample Input
```
5                  
0 2                      
0 4                         
1 3
1 2
1 5
```

### Sample Output
```
3
```

### Hint

$|3−2|+|2−4|=3$ and the last adopter has no pets to adopt.