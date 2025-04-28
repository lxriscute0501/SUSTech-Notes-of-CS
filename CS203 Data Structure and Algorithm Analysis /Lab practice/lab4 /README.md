# lab 4

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Brackets Matching

### Description

There are $n$ brackets, and you want to know whether they are matched. The brackets will only contain $\{, \}, (, ), [, ]$. Here is the matching rules. For example, $\{\{[\}]\}$ is not matching, but $[\{\{\}\}()]$ is matching. Please write a program to check whether the given string is matched or not. 

### Input

The ﬁrst line will be an integer $T$, which is the number of test cases. $(1 \leq T \leq 10)$.

For each test case, the ﬁrst line will be an integer $n \ (1 \leq n \leq 30000)$.

Then there is a line with $n$ brackets. 

### Output

For each test case, print `YES` if the test case is a matching case. Otherwise, print `NO`. 

### Sample Input
```
7
1
(
2
()
2
{]
6
[(){}]
4
(])[
8
[[{{}}]]
6
[][{]]
```

### Sample Output
```
NO
YES
NO
YES
NO
YES
NO
```

<br>

## B. Playroom

### Description

LowbieH and Peggy are in the playroom to figure out an interesting problem. Given a string of matched parenthesis $s$, the scoring rules are defined by:

1. If $s=()$, then $score(s)=1$.

2. If $t$ is another string of matched parenthesis, then $score(s+t)=score(s)+score(t)$.

3. $score((s))=2 \cdot score(s)$.

LowbieH and Peggy want to calculate the score of given string quickly, can you help them? Since the answer may be very large, you only need to tell them the score after $\mod 514329$.

### Input

One line a string. It contains only parenthesis and is perfect matched. The length of input will not exceed $100,000$.

### Output

One line an integer, denoting the score $(\mod 514329)$.

### Sample Input
```
((()())())
```

### Sample Output
```
10
```

### Hint

$score( \ ((()())()) \ )=((1+1)\cdot2+1)\cdot2=10$

<br>

## C. Library

### Description

In the library, LowbieH is learning the double-ended queue, which is also called deque. He thinks that it is very interesting and invites you to join him. Now there are $n$ empty deques numbered from $1$ to $n$, you need to implement three types of operations that LowbieH asks you.

**Type-1:** $1 \ u \ w \ val$. Insert $val$ to the deque numbered by $u$. ($w=0$ means that the insertion is done in the front, $w=1$ means that the insertion is done in the rear).

**Type-2:** $2 \ u \ w$. Query the front or the rear element in the deque numbered by $u$ and pop it out. ($w=0$ means the front, $w=1$ means the rear).

**Type-3:** $3 \ u \ v \ w$. Connect the deque numbered by $v$ to the rear of the deque numbered by $u \ (u \neq v)$. ($w=0$ means a direct connection, $w=1$ means a reversed connection, that is to say you need to first reverse the deque numbered by $v$ and then connect them) The deque numbered by $v$ will be cleared after the connection.

### Input

**Multiple test cases.** Please process to the **end of file**.

For each test case, the first line contains two integers $n \ (1 \leq n \leq 10^5)$ and $q \ (1 \leq q \leq 10^5)$, denoting the number of the deques and the number of the operations. The following $q$ lines will be the three types of operations that have been explained.

$1 \leq u,v \leq n, \ 0 \leq w \leq 1, \ 1 \leq val \leq 10^5$.

It is guaranteed that the total number of operations will not exceed $3\cdot 10^5$.

### Output

Print one line an integer denoting the answer for each **type-2** operation. If the deque is empty, then print `-1` instead.

### Sample Input
```
2 10
1 1 1 23
1 1 0 233
2 1 1 
1 2 1 2333
1 2 1 23333
3 1 2 1
2 2 0
2 1 1
2 1 0
2 1 1
2 10
1 1 1 23
1 1 0 233
2 1 1 
1 2 1 2333
1 2 1 23333
3 1 2 1
2 2 0
2 1 1
2 1 0
2 1 1
```

### Sample Output
```
23
-1
2333
233
23333
23
-1
2333
233
23333
```

### Hint

The data is **randomly generated**, so you can assume that the expected length of each deque is very small.

You are encouraged to self-study the implementation of **deque**.

<br>

## D. Exciting Spider

### Description

Ancient Spider is a very popular card game, and Vince loves to play it. Today he wants to play Ancient Spider again, but he changes the rule to make it more exciting. At the beginning of the game, Vince has an empty slot on the table. There are $n$ different cards numbered from $1$ to $n$, and Vince will receive them one by one in a given order and put the cards onto the top of the slot. At any time, Vince can pick up a card from the top of slot and discard it. If Vince discards all $n$ cards, the game is over. Vince wants you to help him find the smallest lexicographical order among all possible discarding orders at the end of the game.
If you don't know the concept of lexicographical order, you can see the reference in the following link: [lexicographical order](https://en.wikipedia.org/wiki/Lexicographical_order).

### Input

The first line is an integer $T$, which is the number of test cases.

Each test case contains two lines. The first line has an integer $n$. The second line contains a sequence $A$ of length $n$, which is a permutation of $1$ to $n$, representing the order Vince receives the cards.

$1 \leq T \leq 5, \ 1 \leq n \leq 300000$.

### Output

For each test case, print $n$ integers in a line, which is the order discarding the card with the smallest lexicographical order.

### Sample Input
```
2
3
1 3 2
4
3 4 1 2
```

### Sample Output
```
1 2 3
1 2 4 3
```

<br>

## E. Magic Number

### Description

There is a queue of $n$ students, indexed from $1$ to $n$ from left to right. The height of every student has been given. Because students are standing in one straight line, for every student $A_i$ in the queue, he can only see whom between him and the first student who is taller than him whether he looks left or right. Today teacher wants every student to find two partners, who are the highest ones the student can see when he looks left and right respectively. Please help students find their partners. Notice that for every student, the partners must be shorter than himself.

### Input

The first line is integer $T \ (1\leq T\leq 1000)$, the number of test cases. 

Each test case consists of two lines. 

The first line is an integer $n \ (0 < n \leq 50000)$ which represents the number of students. 

The second line lists the heights of students from left to right. It is guaranteed that heights of students are less than $2^{31}-1$ and no two students share the same height in one queue.

### Output

For each case, print the case number in one line. 

Then for every student in the testcase, print the index of his two partners in one line seprated by whitespace. If the eligible partner can not be found, the index should be $0$.

 For example, for the student of height $5$ in first testcase, he can not see anyone on his left so he can not find left parter and index should be $0$. And because he is the highest one in the queue, he can see all the others on his right and the tallest one will be chosen as his right partner. so he choose the student with height $4$ and index $3$.

### Sample Input
```
2
5
5 2 4 3 1
5
2 1 4 3 5
```

### Sample Output
```
Case 1:
0 3
0 0
2 4
0 5
0 0
Case 2:
0 2
0 0
1 4
0 0
3 0
```

<br>

## F. Fencing Hall

### Description

Dateri has a magic sequence and LowbieH is interested in it. Dateri promises that if LowbieH can answer his question, then he will play fencing with LowbieH. We denote the magic sequence by $\{a_n\}$ and Dateri will choose a lucky number $k$. He asks LowbieH to find length of the longest **consecutive subsequence** such that the absolute difference between any two number in the subsequence should not exceed $k$. Can you help LowbieH?

### Input

The first line contains two integers $k \ (0 \leq k \leq 2\cdot 10^9)$ and $n \ (1 \leq n \leq 3\cdot 10^6)$.

The second line contains $n$ integers $a_1, ..., a_n \ (1 \leq a_i \leq 2\cdot 10^9, \ 1 \leq i \leq n)$.

### Output

One line contains the answer, i.e. the length of the longest available consecutive subsequence.

### Sample Input
```
3 9
5 1 3 5 8 6 6 9 10
```

### Sample Output
```
4
```

### Hint

There are two available consecutive subsequences: $\{5,8,6,6\}$ and $\{8,6,6,9\}$.
