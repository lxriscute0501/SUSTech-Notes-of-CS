# lab0 Welcome Lab
Score: A-15 B-15 C-20 D-20 E-25 F-25

## A. Search Problem I

### Description

Given two arrays ``A`` with length ``n`` and ``B`` with length ``T``. We want to know whether each element in array ``B`` is in array ``A`` or not.

### Input

The 1st line is a positive integer ``n (1 <= n <= 1000)``.     
The 2nd line contains ``n`` integers: ``a1, a2, ..., an``. For each ``ai``, ``0 <= ai <= 1e9``.     
The 3rd line contains a positive integer ``T (1 <= T <= 1000)``.        
The 4th line contains ``T`` integers ``b1, b2, ..., bT``. For each ``bi``, ``0 <= bi <= 1e9``.     

### Output

For each element in ``B``, print **"yes"** (in a single line, without quotes) if it is in ··A··, otherwise print **"no"**

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

Given two arrays ``A`` with length ``n`` and ``B`` with length ``T``. We want to know whether each element in array ``B`` is in array ``A`` or not.   
Since the input is large, you may need fast I/O (in Java). Java Faster I/O is shown via following links: [Java Faster I/O](https://pastebin.ubuntu.com/p/zBd3g4j366/)

### Input

The 1st line is a positive integer ``n (1 <= n <= 1e6)``.     
The 2nd line contains ``n`` integers: ``a1, a2, ..., an``. For each ``ai``, ``0 <= ai <= 1e5``.     
The 3rd line contains a positive integer ``T (1 <= T <= 1e6)``.        
The 4th line contains ``T`` integers ``b1, b2, ..., bT``. For each ``bi``, ``0 <= bi <= 1e5``.   

### Output

For each element in ``B``, print **"yes"** (in a single line, without quotes) if it is in ``A``, otherwise print **"no"**

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

## C. Majsoul

### Description

Mahjong, one of the most famous games in China, has aroused lanran’s interest. However, it is a little bit complex for a new bee and he needs your help: To sort 13 Mahjong tiles in order, so that he can make decision much easier. Here is the rule of sorting Mahjong tiles:

1. Mahjong tiles can be roughly divided into four types, ‘萬’, ‘筒’, ’条’, ‘字’. For the first three types, each type has 9 different numbers(‘1’ to ‘9’), usually noted by ‘1萬’, ‘2萬’, ’1条’, ’2条’. If you still get puzzled, just imagine that we express the cards in ‘UNO’ by a number and a Chinese character rather than a colour. For the last type, there are only 7 kinds of tiles, ‘東’, ‘南’, ‘西’, ‘北’, ’白’, ’發’, ’中’.

2. ‘萬’ noted by ‘Wx’ (x is an integer between ‘1’ to ‘9’), for example (W1, W2, ...). Similarly, ‘筒’ by ‘Tx’, ‘条’ by ‘Yx’. We name those 7 tiles of the last type by ‘E’, ’S’, ’W’, ’N’, ’B’, ’F’, ’Z’ correspondingly.

3. Here is the priority of Mahjong tiles: ``Wx > Tx > Yx > E > S > W > N > B > F > Z``      
For the same type: ``W1 > W2 > W3 > W4 > W5 > W6 > W7 > W8 > W9``     

Notice that: For each kind of tile, there are totally four duplicate ones.

### Input
The first line of input is the number of test cases ``T (1 <= T <= 200)``        
For each test case, there are 13 strings in one line, showing the Mahjong tiles you have.

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


## D.  Maximum difference

### Description


### Input


### Output


