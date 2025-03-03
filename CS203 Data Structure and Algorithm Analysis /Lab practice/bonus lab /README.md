# bonus lab

Score: A~J-10

<br>

## A. Split Sticks

### Description

Alice has a bundle of sticks. She wants to select some sticks and split them into `k` rows.  

For beauty, the difference between the lengths of all sticks in the same row shall not exceed `1` and each row has the same number of sticks. The length of each stick is a positive integer from `1` to `n`.      

For every possible length, you know the amount of stick with that length.      

Please calculate the maximum number of sticks Alice can select.    

### Input

The first line is a positive integer `T (1 <= T <= 10000)` which is the number of test cases.

For each test case:       

The first line contains two integers `n (1 <= n <= 30000)` and `k (1 <= k <= 1e12)`, representing the number of different length of sticks and the number of rows the stick needs to be divided into, respectively.       

The second line contains `n` integers `C1, C2, ...,Cn (0 <= Ci <= 1e12)`, representing the number of sticks in each length.    

Ensure that the sum of `n` of all cases does not exceed `30000`.     

### Output

For each case, output the maximum number of sticks Alice can select.    

### Sample Input
```
5
3 4
7 1 13
1 1
100
1 3
100
2 1
1000000000000 1000000000000
4 1
10 2 11 1
```

### Sample Output
```
16
100
99
2000000000000
13
```

### Hint (sample explanation)

1. The arrangement is [3, 3, 3, 3], [1, 2, 1, 1], [1, 1, 1, 1], [3, 3, 3, 3] (each list represents a row).

2. All the sticks can be arranged in the same row.

3. 33 sticks with length 1 in each 3 rows.

4. All the sticks can be arranged in the same row.

5. All sticks with lengths of 2 and 3 are arranged in the same row.

<br>

## B. Discount

### Description

Satori, the bunny store owner, decided to offer a discount to whoever answered her problem:

What's the number of different Max-heaps build on ``N`` different key values?

Note that two heaps are considered different if and only if the two binary trees have different pre-order traversals.


### Input

A single integer `N (1 <= N <= 1000)`.

### Output

Output a single integer indicating the number of different Max-heaps module `998244353`

> 998244353 is a prime number.


### Sample Input
```
5
```

### Sample Output
```
8
```

<br>

## C. bonus string problem

### Description

Cache is an important concept in Computer Organization (CS202). 
The original meaning of cache is a kind of high-speed memory whose access speed is faster than general random access memory (RAM).
Generally, it does not use DRAM technology as systems main memory, but uses expensive but faster SRAM technology.
The setting of cache is one of the most important factors for all modern computer systems to achieve high performance.
A hit means the queried item is already in cache before the query reaches while a miss means the item is not in cache and after the query it will be stored in cache. 
The cache have a cache size `C`. When a cache store items more than `C`, it will adopt Least-Recently-Used (LRU) algorithm to replace item stored in cache.
Here we simplify the concept, a cache can store at most `k` items. When the cache is full and a new item need to store in cache, it will replace the item that is least recently used.

QR code, full name quick response code, is a super popular coding method on mobile devices in recent years. 
It can store more information than the traditional bar code barcode, and can also represent more data types. 
To simplify the problem, We assume QR code is a `16 * 16` matrix contains only `0` and `1`.

Now the question is coming. Dateri is going to deal with `n` QR codes and he have a cache size `k`. For each query QR code, he wants to know whether the query is hit or miss.


### Input

First line contains two integer `n (1 < n <= 1000)` and `k (1 <= k <= n)`.

Continue with `n` query, each query contains a `16 * 16` matrix which consist of `0` and `1`.

### Output

For each query, outputs whether hit or miss.    

### Sample Input
```
6 2

1 1 0 0 1 0 0 0 0 0 1 1 1 1 1 1
1 0 1 0 1 0 0 1 0 0 1 0 0 1 1 0
1 0 1 0 1 1 1 0 1 1 0 1 1 0 1 1
1 0 1 0 0 1 1 1 1 1 1 0 0 1 0 0
0 0 0 0 0 0 0 1 0 1 0 0 0 1 1 0
1 1 0 0 0 0 0 0 1 0 0 1 0 1 1 0
0 0 1 1 1 1 1 0 0 0 1 0 1 0 1 1
0 0 0 1 1 1 1 0 0 0 1 0 1 1 1 0
1 0 0 0 1 0 0 0 1 1 1 1 1 1 1 1
1 1 1 0 1 0 0 0 0 0 1 0 0 1 0 1
0 1 0 1 0 1 1 1 0 0 1 0 0 0 0 1
0 1 0 0 1 0 1 1 0 0 0 0 1 1 0 1
0 1 1 1 0 1 1 0 1 0 1 1 0 1 1 0
0 1 0 0 0 1 1 0 1 1 1 1 1 1 0 1
0 0 0 0 0 0 0 1 1 0 1 1 0 0 0 0
0 1 0 1 0 1 1 0 0 1 0 0 0 1 0 0



0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0



1 1 0 0 1 0 0 0 0 0 1 1 1 1 1 1
1 0 1 0 1 0 0 1 0 0 1 0 0 1 1 0
1 0 1 0 1 1 1 0 1 1 0 1 1 0 1 1
1 0 1 0 0 1 1 1 1 1 1 0 0 1 0 0
0 0 0 0 0 0 0 1 0 1 0 0 0 1 1 0
1 1 0 0 0 0 0 0 1 0 0 1 0 1 1 0
0 0 1 1 1 1 1 0 0 0 1 0 1 0 1 1
0 0 0 1 1 1 1 0 0 0 1 0 1 1 1 0
1 0 0 0 1 0 0 0 1 1 1 1 1 1 1 1
1 1 1 0 1 0 0 0 0 0 1 0 0 1 0 1
0 1 0 1 0 1 1 1 0 0 1 0 0 0 0 1
0 1 0 0 1 0 1 1 0 0 0 0 1 1 0 1
0 1 1 1 0 1 1 0 1 0 1 1 0 1 1 0
0 1 0 0 0 1 1 0 1 1 1 1 1 1 0 1
0 0 0 0 0 0 0 1 1 0 1 1 0 0 0 0
0 1 0 1 0 1 1 0 0 1 0 0 0 1 0 0



0 0 0 0 0 0 1 1 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 1 0 0 0 1 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 1 1 0 0 0 0 0 0 0 0 0
1 0 0 1 0 0 0 0 0 0 0 0 0 0 1 0
0 0 0 0 0 0 0 0 1 0 0 0 0 0 1 1
0 0 0 0 0 1 0 0 0 1 0 0 1 0 0 0
0 0 0 0 0 0 1 0 1 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 1
1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 1 0 0 0 0 0 0 0 1 0 0 1 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0
0 0 0 0 0 1 0 0 0 1 0 1 1 0 0 0
0 0 0 0 0 0 0 0 0 1 0 0 0 1 0 0



0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0



0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
```

### Sample Output
```
miss
miss
hit
miss
miss
hit
```

<br>

## D. Dateri's Investment In Stocks

### Description

Dateri is a smart boy who is thirsty for making a big fortune. 
Recently, he decides to invest in stocks to try his luck. 
Unfortunately, due to the miserable state of the economy and lack of knowledge and experience, he suffered a great loss of money. 
He has no choice but to ask LowbieH for help. 

LowbieH offers him a predictor which can forecast the stock index of the upcoming `n` days. 
Dateri is very excited and can't wait to make money. 
He wants to know the minimum days that he needs to wait to get a rise in the index if making a buy-in on the `k-th` day.

### Input

The first line is an integer `T (1 <= T <= 12)`, meaning the number of the test cases.

For each test case, an integer `n (1 <= n <= 2e5)` denotes the number of the forecasted upcoming days, followed by `n` stock indexes for each day: `index1, ..., indexn (1 <= indexi <= 1e9, 1 <= i <= n)`.

The next line contains an integer `q (1 <= q <= 2e5)` denoting the number of the queries. For each query, there's an integer `k`, which means that Dateri will make a buy-in on the `k-th` day.


### Output

Print the answer of each query in `q` lines. If Dateri can't get a rise within the upcoming `n` days, print `−1`.

### Sample Input
```
1
5
2 3 3 5 4
5
1 2 3 4 5
```

### Sample Output
```
1
2
1
-1
-1
```

### Hint

"rise" should be understanded as "strictly bigger than".

Huge input and output. You are recommended to use fast I/O.

<br>

## E. Build a Binary Tree & Traverse it! 

### Description

In this problem, you need to:

1. Implement a Binary Tree **using a linked list** (as explained during the lecture) to encode a mathematical expression with binary operators (`+, -, *, /`) provided in input in prefixnotation i.e. functional programming notation.
You should use a stack to keep track of the pointers of the nodes that you will need later to fill in their right child.

2. Implement the procedures **INORDER, PREORDER and POSTORDER** and print the respective outputs when applied to the resulting tree form Step 1.

Example input: `-(+(a,*(b,c)),/(d,e))` which leads to the tree given in the lecture slides

**VERY IMPORTANT HINTS:**

1. You should use a linked list to implement the binary tree, like:
   
   ```cpp
   class node {
	node LeftSon;
	node RightSon;
	...; // some info of the node
   };
   ```
   ```cpp
   struct node {
	node* LeftSon;
	node* RightSon;
	...; // some info of the node
    };
   ```

2. You should implement three methods for the three procedures respectively, based on the binary tree you have built with a linked list.
       **DO NOT** simulate the procedures on arrays instead of linked lists, otherwise you will get a **WRONG ANSWER after the deadline** even if you have passed this problem.
   
3. Actually, you can safely ignore all `'('`, `')'` and `','`  in the input file, since the prefix notation (also known as Polish notation) is ambiguity-free without brackets.

### Input

A prefix notation string with commas and brackets.

All the operands are single lowercase letters, and all the operators are one of `'+', '-', '*', '/'`.

The prefix notation string is always legal (i.e. in correct form).

`1 <= string.length <= 20000`

### Output

Three lines, indicating the result of INORDER, PREORDER and POSTORDER traverse.   

### Sample Input
```
-(+(a,*(b,c)),/(d,e))
```

### Sample Output
```
((a+(b*c))-(d/e))
-+a*bc/de
abc*+de/-
```

<br>

## F. Mex Problem

### Description

Here are `t (1 <= t <= 1e5)` tests, each test contains two integers `a (0 <= a <= 1e9)` and `b (0 <= b <= 1e9)`, you should find the Mex of the sequence `a⊕0, a⊕1, ...,a⊕b`, here `⊕` mean **the bitwise xor** operation. 

The Mex of the sequence of non-negative integers is the smallest non-negative integer that doesn't appear in this sequence.
For example, `Mex(1, 2, 3) = 0, Mex(0, 1, 2, 4, 5) = 3`.


### Input

The first line contains a single integer `t (1 <= t <= 1e5)`, indicates the number of test cases.    

The first and only line of each test case contains two integers `a` and `b` `(0 <= a, b <= 1e9)`.   

### Output

For each test case, print a line contains a single integer  — the answer to the problem.

### Sample Input
```
3
0 9
8 6
9 9
```

### Sample Output
```
10
0
2
```

<br>

## G. Exchange

### Description

Give you `n` linklists, each with`m` numbers inside. The chains index are numbered `0` to `n-1`, and the nodes on the initial `i-th` chain are numbered `(i*m), (i*m+1), ......,(i*m+(m-1))`

After that you are given `k` operations, each operation will select two node numbers `a, b`. Disconnect the right side of `a` and the left side of `b` on the current linklist into four parts, then join `a` and `b` together and join the remaining two disconnected parts together.

For example, `n = 3, m = 5`. If you choose `a, b = 1, 7` then the result after reconnection is `[5, 6, 2, 3, 4]` and `[0, 1, 7, 8, 9]`.

After these operations, ask for all the nodes of the linklist where node `x` is located and output them from left to right.   

### Input

First line 4 numbers: `n, m, k, x  (n*m < 1e6, 0 < k < 2e5)`.  

Then follow `k` lines, each line two numbers: `a, b (0 <= a, b < n*m)` meaning the nodes selected.   

> It is guaranteed that the two nodes selected each time are not currently in the same chain.

### Output

A line containing a lot of numbers of all the index of nodes in the linklist where node `x` is located and output them from left to right.


### Sample Input
```
3 5 1 0
1 7
```

### Sample output
```
0 1 7 8 9
```

<br>

## H. Skiing (Easy-15)

### Description

Yuki is an outgoing girl and she enjoys all the sports, especially snow sports like skiing.

Now she is skiing in a world-famous ski resort — Super Ultra Ski Training Center (SUSTC). The map of SUSTC can be considered as a grid map with `n` rows and `m` columns. Since the ski facility is uneven, each grid `(i, j)` has its own height `h(i, j)`.

Yuki starts her skiing at the grid `(1, 1)` — the top left grid, and her destination is at the grid `(n, m)` — the bottom right grid. Each time she can only ski to one of the adjacent grids — the left, the right, the above, or the below grid. Obviously, the minimum distance for Yuki to ski to the destination is `n + m - 2` if she chooses an arbitrary Manhattan path.

However, with the elevation change during her skiing, Yuki’s speed will also change remarkably. Specifically, suppose that the velocity of Yuki at grid `(x, y)` is `v`, then it takes `1 / v` time units for Yuki to move to `(x, y+1)`, and the velocity at grid `(x, y+1)` will be changed to `v * 2^(h(x, y) − h(x, y+1))`. Suppose that the initial velocity when Yuki at grid `(1, 1)` is `v0 = 1`, please tell Yuki the minimum time for her to arrive at the destination.

### Input

The first line contains two integers: `n` and `m` `(1 <= n, m <= 300)` — size of the grid map.

Each of the next `n` lines contains `m` integers. The `j-th` number in the `i-th` line denotes `h(i, j) (1 <= h(i, j) <= 15)`.

### Output

Print one line with the value — the minimum time for Yuki to arrive at the destination.

The value should be accurated to exactly 2 decimal places.

### Sample Input
```
2 3
1 2 3
4 5 6
```

### Sample Output
```
7.00
```

<br>

## I. Minimum pearls

### Description

There are two kinds of pearls: black and white. When a black pearl is the left of a white pearl and they are adjacent to each other, then they will disappear. 

Now, there are `n` strings of pearls, each string with `ai` white pearls on the left and `bi` black pearls on the right. Calculate the minimum number of the rest pearls when connecting these `n` strings.

### Input

The first line contains an integer `T`, indicating the number of test cases. 

For each test case:

The first line contains an integer `n (1 <= n <= 1e6)`, indicating the number of the string of beads.

Each of the next `n` lines contains two integers `ai, bi (ai >= 0, bi >= 0, 1 <= ai + bi  <= 10,000).

It is guaranteed that `∑(ai + bi) <= 500,000`.   

### Output

Output one integer, indicating the minimum number of the rest pearls.


### Sample Input
```
2
2
1 2
2 1
2
1 3
2 1
```

### Sample Output
```
2
3
```

<br>

## J. Courier
> No one solved.

### Description

Van is a courier who needs to deliver `n` parcels numbered from `1` to `n`. When a parcel is delivered for the `i-th` time, we define its delivery priority to be `i`.

There are two restrictions on the order of delivery of parcels:     
A parcel numbered `i` has a delivery priority less than `ki`;      
There is some restriction `(i, j)` that the `i-th` parcel must be delivered earlier than the `j-th` parcel.    

Please help us to calculate the minimum delivery priority of each parcel among all possible options.

### Input

The first line contains two positive integers `n (n <= 2000)` and `m (m <= 10000)`, where `n` represents the number of packages, and `m` represents the number of restrictions of the second type.

The second line contains `n` positive integers `k1, k2, ...,kn` and the next `m` lines, each line contains two positive integers `a` and `b`, representing a pair of relative take-off order restrictions `(a, b)`, where `a, b (1 <= a, b <= n)`, which means that package `a` must start before package `b`.

### Output

Contains `n` integers `t1, t2, ...,tn` represents the smallest possible starting sequence number for package `i`, and two adjacent integers are separated by spaces.


### Sample Input
```
5 5
4 5 2 5 4
1 2
3 2
5 1
3 4
3 1
```

### Sample Output
```
3 4 1 2 1
```

