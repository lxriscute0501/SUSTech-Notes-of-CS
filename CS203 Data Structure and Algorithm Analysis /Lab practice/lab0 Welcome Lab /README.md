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

