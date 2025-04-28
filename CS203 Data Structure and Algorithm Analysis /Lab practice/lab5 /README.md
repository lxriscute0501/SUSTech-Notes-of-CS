# lab 5

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. How many substrings

### Description

Give you a string, you should print how many none empty substrings it has.

### Input

The first line is number of tests. $T \ (1 \leq T \leq 10)$

The second line is a string $S$. The length of $S$ doesn’t exceed $1000$, that is $|S| \leq 1000$.

$S$ will only contain lower case English letters.

### Output

For each test, you should print an integer in a single line, which is the number of none empty substrings of $S$.

### Sample Input
```
1
hello
```

### Sample Output
```
15
```

<br>

## B. [Easy] Find Next

### Description

KMP algorithm is an improved string matching algorithm proposed by D.E.Knuth, J.H.Morris and V.R.Pratt, so it is called Knut Morris Pratt operation (KMP algorithm for short). The core of KMP algorithm is to use the information after matching failure to reduce the matching times between pattern string and main string to achieve the purpose of fast matching. The specific implementation is realized by a $next()$ function, which contains the local matching information of pattern string.

Now give you a text $S$, please output its $next$ array.

### Input

A line contains a text string $S$. $|S| \leq 10^6$.

### Output

Output $S.length$ lines. The $i$-th line indicates $next[i]$ for $S$.

### Sample Input
```
ababc
```

### Sample Output
```
0
0
1
2
0
```

<br>

## C. FSA

### Description

As learned in the lecture, FSA can be used to solve string matching problems. Here you are given a string $S$, and you are required to output it's transition function according to the FSA algorithm.

### Input

One line, indicating the input string $S \ (1 \leq |S| \leq 10^5)$. The string contains only lowercase letters.

### Output

$|S|$ lines, each containing $26$ integers, indicating the transition function.

### Sample Input
```
aabaaabb
```

### Sample Output
```
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
2 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
6 3 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
2 7 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
4 8 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
```

<br>

## D. Necklace!

### Description

D1rtyPaw5 wants to drink coco with his girlfriend! Unfortunately, he has no girlfriend now, but he still wishes to prepare a necklace for his future goddess.  

This is a kind of special necklace which contains a sequence of diamonds with letters in, and it can be regarded as a string. D1rtyPaw5 gives Amayama a semi-finished necklace and askes Amayama to add as few as possible diamonds to the head or tail of the necklace to make the necklace consist of **at least two same circulation sections**. A circulation section of a string is a substring which could build the original string by repeating the circulation section several times.  

Please answer the number of diamonds that Amayama should add.

### Input

The first line contains an integer $n$, which is the number of test cases. $(1 \leq n \leq 1000)$ 

For each case, there is only one line of string containing lowercase letters, which indicates the semi-finished necklace. The length of each string is no more than $200,000$.

### Output

The output for each test case is an integer which is the number of diamonds that Amayama should add.

### Sample Input
```
2
wawawa
failed
```

### Sample Output
```
0
6
```

### Hint

If a necklace has contained two circulation sections and ended with an uncomplete circulation section, you should complete this section to finish the whole necklace.

<br>

## E. Stick!

### Description

Lovely boys often play stick game. This is a two-player game, which goal is finding the longest common substring of the two players' names, and the length of it is defined as the stick level. Now, Amayama wants you help him to calculate the stick level between two boys.

### Input

The input contains two string $s1, s2 \ (1 \leq |s1|, |s2|\leq 10^6)$, which are two lovely boys' names.

### Output

A number stands the sticking level.

### Sample Input
```
B.Tang
B.Tarjan
```

### Sample Output
```
4
```

### Hint

[hashing strings](https://en.wikipedia.org/wiki/Universal_hashing#Hashing_strings)

<br>

## F. String Problem F

### Description

There is a string $S_0$ with length $2m$. Its first half $S_0[1,m]$ is the substitution cipher of the second half $S_0[m+1,2m]$. Then for some reason, the second half of $S_0$ may lose some characters at the end, resulting in string $S$.

Given this string $S$, you need to answer the smallest possible value of $m$.

**Hint:** Substitution cipher is an encryption method that replaces letters in the text with other letters. For example, if our encryption rule is $a \to c, \ b \to a, \ c \to b$, then the string "$accb$" will be encrypted into "$cbba$".

### Input

The first line is the code table for the substitution cipher, which contains $26$ letters representing the encrypted letter for each letter in lexicographical order. 

The second line is $S \ (1 \leq |S| \leq 5\cdot 10^5)$.

### Output

One integer, indicating the minimal possible length of the second half.

### Sample Input
```
b c d e f g h i j k l m n o p q r s t u v w x y z a
bcdeabc
```

### Sample Output
```
4
```
