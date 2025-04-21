# lab0 Welcome Lab
Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Search Problem I

### Description

Given two arrays $A$ with length $n$ and $B$ with length $T$. We want to know whether each element in array $B$ is in array $A$ or not.

### Input

The 1st line is a positive integer $n (1 \leq n \leq 1000)$.   

The 2nd line contains $n$ integers: $a_1, a_2, ..., a_n$. For each $a_i$, $0 \leq a_i \leq 10^9$.  

The 3rd line contains a positive integer $T (1 \leq T \leq 1000)$.        

The 4th line contains $T$ integers $b_1, b_2, ..., b_T$. For each $b_i$, $0 \leq b_i \leq 10^9$.     

### Output

For each element in $B$, print **"yes"** (in a single line, without quotes) if it is in $A$, otherwise print **"no"**

### Sample Input
```
4
2 3 999999 1
2
3 99
```

### Sample Output
```
yes
no
```

<br>

## B. Search Problem II

### Description

Given two arrays $A$ with length $n$ and $B$ with length $T$. We want to know whether each element in array $B$ is in array $A$ or not.   
Since the input is large, you may need **fast I/O** (in Java). Java Faster I/O is shown via following links: [Java Faster I/O](https://pastebin.ubuntu.com/p/zBd3g4j366/)

### Input

The 1st line is a positive integer $n (1 \leq n \leq 10^6)$.     

The 2nd line contains $n$ integers: $a_1, a_2, ..., a_n$. For each $a_i$, $0 \leq a_i \leq 10^5$.    

The 3rd line contains a positive integer $T (T \leq 10^6)$

The 4th line contains $T$ integers $b_1, b_2, ..., b_T$. For each $b_i$, $0 \leq b_i \leq 10^5$.   


### Output

For each element in $B$, print **"yes"** (in a single line, without quotes) if it is in $A$, otherwise print **"no"**

### Sample Input
```
4
2 3 999999 1
2
3 99
```

### Sample Output
```
yes
no
```

<br>

## C. Majsoul

### Description

Mahjong, one of the most famous games in China, has aroused lanran’s interest. However, it is a little bit complex for a new bee and he needs your help: To sort $13$ Mahjong tiles in order, so that he can make decision much easier. Here is the rule of sorting Mahjong tiles:

1. Mahjong tiles can be roughly divided into four types, $萬$, $筒$, $条$, $字$. For the first three types, each type has 9 different numbers ($1$ to $9$), usually noted by $1萬$, $2萬$, $1条$, $2条$. If you still get puzzled, just imagine that we express the cards in UNO by a number and a Chinese character rather than a colour. For the last type, there are only $7$ kinds of tiles, $東, 南, 西, 北, 白, 發, 中$.

2. $萬$ noted by $Wx$ ($x$ is an integer between $1$ to $9$), for example ($W1, W2, ...$). Similarly, $筒$ by $Tx$, $条$ by $Yx$. We name those $7$ tiles of the last type by $E, S, W, N, B, F, Z$ correspondingly.

3. Here is the priority of Mahjong tiles: $Wx > Tx > Yx > E > S > W > N > B > F > Z$      
For the same type: $W1 > W2 > W3 > W4 > W5 > W6 > W7 > W8 > W9$ 

Notice that: For each kind of tile, there are totally four duplicate ones.

### Input

The first line of input is the number of test cases $T (1 \leq T \leq 200)$        
For each test case, there are $13$ strings in one line, showing the Mahjong tiles you have.

### Output

For each test case, output the ordered Mahjong tiles in one line.

### Sample Input
```
2
W1 S S N E N E N W E W W S
T1 T2 T3 T5 T8 T9 T6 T4 T7 T9 T9 T1 T1
```

### Sample Output
```
W1 E E E S S S W W W N N N  
T1 T1 T1 T2 T3 T4 T5 T6 T7 T8 T9 T9 T9
```

<br>

## D.  Maximum difference

### Description

Give you $n$ integers $a_1, a_2, ..., a_n$. Please find two integers $a_i$ and $a_j \ (i < j)$, such that $a_i – a_j$ is maximum.

### Input

The first line of input is the number of test cases $T (1 \leq T \leq 10)$    
For each test case, the first line will be an integer $n (2 \leq n \leq 200000)$. Then there are $n$ integers. $|a_i| \leq 100000$.

### Output

For each test case, print the maximum difference.    

### Sample Input
```
2
5
1 2 3 4 5
5
1 2 3 4 5
```

### Sample Output
```
-1
-1
```

<br>

## E. 3-D Print

### Description

Please write a program to print a 3D cuboid with length $a$, width $b$ and height $c$. You can see the details in the sample input and output.

### Input

The first line of input is the number of test cases $T (1 \leq T \leq 100)$
For each test case, the first line will be three integers $a$, $b$ and $c$ $(1 \leq a, b, c \leq 30)$.

### Output

For each test case, print the cuboid as the sample output.

### Sample Input
```
2
1 1 1
6 2 4
```

### Sample Output
```
..+-+
././|
+-+.+
|.|/.
+-+..
....+-+-+-+-+-+-+
.../././././././|
..+-+-+-+-+-+-+.+
./././././././|/|
+-+-+-+-+-+-+.+.+
|.|.|.|.|.|.|/|/|
+-+-+-+-+-+-+.+.+
|.|.|.|.|.|.|/|/|
+-+-+-+-+-+-+.+.+
|.|.|.|.|.|.|/|/.
+-+-+-+-+-+-+.+..
|.|.|.|.|.|.|/...
+-+-+-+-+-+-+....
```

<br>

## F. Mahhjong

### Description

Please determine whether one can win with the $14$ tiles in her hands or not.

In Mahhjong, there are $34$ kinds of tiles, which are divided into four suites, named as *bing*, *suo*, *wan*, and *zi*. The *bing*, *suo*, *wan* have $9$ kinds for each suite and *zi* tiles has only $7$ kinds. To simplify the problem, each tile is represented with a number and a suite, for example $1b, 2s, 7w, 3z$.

The rules in Mahhjong are similar with those in Mahjong, except that in Mahhjong there is an infinite number of each kind, while in Mahjong one kind usually contains up to $4$ tiles. Also, we consider a combination of tiles winning if and only if the combination consists of four *kezi* or *shunzi* and an additional *quetou*.

If you are not familiar with Mahjong, here is a brief explanation:

- **kezi**: *kezi* is a set of $3$ identical tiles.

  For example, $\{1s, 1s, 1s\}$, $\{3z, 3z, 3z\}$ are *kezi*, but $\{1s, 2s, 1s\}$ is not.

- **shunzi**: *shunzi* is a set of $3$ continuous tiles of the same suite, but please aware that suite *zi* cannot form *shunzi*.

  For example, $\{1s, 2s, 3s\}$, $\{6b, 7b, 8b\}$ are *shunzi*, but $\{5z, 6z, 7z\}$ and $\{3w, 4w, 5s\}$ are not.

- **quetou**: *quetou* is a pair of identical tiles.

  For example, $\{7z, 7z\}$, $\{6w, 6w\}$ are quetou, but $\{1b, 2b\}$ is not.

- Special combination (winning status): we consider a combination of tiles winning if and only if the combination consists of four kezi or shunzi and an additional quetou.

  For example, $\{1w, 2w, 2w, 2w, 3w, 4b, 5b, 5b, 6b, 6b, 7b, 9s, 9s, 9s\}$ is a special combination, because we can divide the set of tiles into three *shunzi*, a *kezi* and a *quetou*: $\{\{1w, 2w, 3w\}, \{4b, 5b, 6b\}, \{5b, 6b, 7b\}, \{9s, 9s, 9s\}, \{2w, 2w\}\}$

### Input

The input contains multiple test cases. 

The first line includes a single integer $T (1 \leq T \leq 10000)$ -- the number of test cases.

Each of the next $T$ lines indicates a test case. It contains a string $s$ of $28$ characters, describing the $14$ tiles that Satori has. For every $1 \leq i \leq 14$, the $i$-th tile  is described by the $(2i − 1)$-th and $2i$-th characters in the string: the former is a digit denoting the rank of the tile in its suite and the latter is one of $\{w, b, s, z\}$, which means the suite *wan*, *bing*, *suo* and *zi* respectively. It is guaranteed that all the $s$ in the input are valid and legal.

### Output

For each test case, output a single line. If one reached the winning status, print **"Blessing of Heaven"**, otherwise print **"Bad luck"** (without quotes).  

### Sample Input
```
5
1w2w3w4b5b6b7s8s9s1b1b1z2z6z
1w2w3w4b5b6b7s8s9s1b1b2z2z6z
1w2w3w4b5b6b7s8s9s1b1b2z2z2z
1b2b3b4b5b6b2s4s5s5s5s6s7s8s
1b1b1b2b3b4b5b6b7b8b9b9b9b1s
```

### Sample Output
```
Bad luck
Bad luck
Blessing of Heaven
Bad luck
Bad luck
```
