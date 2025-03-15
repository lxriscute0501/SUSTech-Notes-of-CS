# lab 3

Score: A-15 B-15 C-20 D-20 E-25 F-25

<br>

## A. Eating Ordering

### Description

K‘, is a well-known character in The King of Fighters series developed by SNK. 
Dateri likes K' so he likes number `k`. 
One day he enters a restaurant and orders `n` dishes. 
He wants to eat them one by one.
He has an idea to decide the order of eating: 
Dishes are in a circle, numbered from `1` to `n` in clockwise direction. 
Dateri begins counting at dish `1` and proceeds around the circle in clockwise direction.
When he skips `k − 1` dishes, he will eat the next dish. 
The procedure is repeating until there is no dishes.
You should output the ordering of dishes Dateri eats.

### Input

The first line of input contains an integer `T (1 <= T <= 10)` which is the total number of test cases.

For each test cases, there are two integers in one line, `n` and `k` `(1 <= n <= 1e5)`.

It satisfies that in all cases `∑ n * k <= 6e7`.

### Output

For eact test case output a line.

`n` integer in one line, representing the ordering of dishes Dateri eats.

### Sample Input
```
1
5 3
```

### Sample Output
```
3 1 5 2 4
```

<br>

## B. Add polynomials together

### Description

As Narnal has learned in the class, a polynomial can be represented as a linked list, but he failed to add two given polynomials together. He asks you to help him to get the summation of two polynomials.

### Input

First line will be a positive integer `T`, which is the number of test cases.

In each test case, the first line will be an integer `n` for the number of terms of the first polynomial. Then the next `n` lines will be the coefficients and exponents of the terms with the order of **increasing** exponents.

After `n + 1` lines, there will be an integer `m` for the number of terms of the second polynomial. Then the next `m` lines will be the coefficients and exponents of the terms with the order of **increasing** exponents.

After `n + m + 2` lines, there will be an integer `q` denoting the number of queries. Then one line containing `q` **ascending** numbers will follow, where each number `k` represents a query for the coefficient of exponent of `k`.

`1 <= T <= 200`, `1 <= n, m <= 1,000`, `1 <= q <= 2,000`, `0 <= k <= 1e9`. The coefficients will be in range `[−10,000, 10,000]`. The exponents will be in range `[0, 1e9]`.

### Output

For each case, output one line contains `q` answers of the corresponding queries.

### Sample Input
```
1
2
1 1
2 3
3
1 100
3 150
5 170
4
1 20 100 170
```

### Sample Output
```
1 0 1 5
```

<br>

## C. Black era

### Description

The war in Ursus has broken out! Now, Wamiya and her teammates have to escape from this dangerous place immediately. In order to avoid the falling objects from the surrounding buildings, all the team members are lined up in a row while moving forward. In order to defend against enemy attacks, Wamiya should also constantly change the form of the team by exchanging two parts of the row, i.e., she will specify two continuous parts in the row, and exchange their locations without changing anything inside each part. There will be `N` members in the team, and Wamiya will give `M` orders one by one. Given the initial team, what will be the final team after their escape?

### Input

First line contains one integer `T`, indicating that there will be `T` cases.

For each case, the first line will be two integers `N` and `M`. `N` is the number of the members in the team, and `M` is the number of orders.

The second line will be `N` integers, indicating the initial formation of the team. Each number is unique and represents the ID of the member. We guarantee that ID is in the range of `[0, N − 1]`.

The following `M` lines represent `M` orders. In each line, there are four integers `x1, y1, x2, y2`, which mean the part `[x1, y1]` (starting from member with ID `x1` and ending at the member with ID `y1`), should exchange the position with part `[x2, y2]`.

`T <= 5`, `1 <= N, M <= 1e5`. We guarantee that in each order, members with ID `x1, y1, x2, y2` will be arranged in order from front to back in the row. Notice that `x1` could be equal to `y1` and `x2` could be equal to `y2`.

### Output

For each case, output `N` integer in one line to represent the final team consisting of member IDs.

### Sample Input
```
1
10 2
0 6 5 1 2 3 4 8 7 9
6 4 7 9
0 8 6 5
```

### Sample Output
```
6 5 0 7 9 8 1 2 3 4
```

<br>

## D. Coverted Vinux input

### Description

Narnal is a huge fan of vim, so he has created a text editor called Vinux, which can only edit one line with several operations. Each line has an invisible undeletable tail character called EOL (end of line), which will always stay at the end of the line in any circumstance. **Notice that the undeletable property of EOL means that EOL will revive immediately at the end of the line whenever it vanishes (been replaced or deleted)**. He wants to convert a **one-line** keyboard input containing operations and digits into a **one-line** real input with only digits.

Only the following operations are available:

``r``: next single input would replace the current character;

``I``: move the character pointer to the head of the line;

``H``: left shift the current character pointer unless it is at the leftmost place;

``L``: right shift the current character pointer unless it is at the rightmost place;

``x``: delete the current character;

Otherwise, each input would insert before the current character.

### Input

First line will be a positive integer `T`, which is the number of test cases.

In each test case, the first line would be an integer `n` for the length of the keyboard input of Vinux. Then the following line represents the keyboard input of Vinux.

`T <= 20`, `20 <= n <= 10,000`. The aiming real input only contains digits without blanks. The input would always be valid (the input after ``r`` would never be an operation character).

### Output

For each case, output one line shows the real input without EOL.

### Sample Input
```
2
25
12345HHHr9Ir000LLLLL876Ix
20
r60xxxxHx4xHH3III1I2
```

### Sample Output
```
002945876
2134
```

<br>

## E. From-now-on minimum difference

### Description

Yuki is a clever girl and she is good at mathematics. One day, she gets an array `a` of `n` integers: `a1, a2, ..., an`. She wants to know the from-now-on minimum difference of `a1, a2, ..., a(n-1)`, and your task is to help her to calculate them. The from-now-on minimum difference of `ai`, denoted by `hi`, is defined as: 
```math
hi = min|aj − ai| (i < j)
```

### Input

The first line contains one integer: `n (2 <= n <= 2e6)`. The second line contains `n` space-separated integers: `a1, a2, ..., an` — elements of the array `a` `(1 <= ai <= 1e9)`.

### Output

Print one line with `n − 1` space-separated intergers: `h1, h2, ..., h(n−1)`.

### Sample Input
```
5
1 2 3 4 5
```

### Sample Output
```
1 1 1 1
```

### Hint

You may solve this problem using some ~~advanced data structures~~. However, it can be solved in a simple and efficient way merely by **sorting algorithm** and **linked list**.

Please note that the size of input might be really large, so you might want to use an efficient way to read the input data.

<br>

## F. Peggy hates decreasing

### Description

Peggy is an aggressive boy, and he hates decreasing. So he wants to delete all decreasing number. He has a sequence `{ai}` of length `n`, and the definition of decreasing number is the number either the previous one is bigger than it or the next one is smaller than it. In other word, the number of index `i` is a decreasing number if and only if `i > 1 && a[i − 1] > a[i]` or `i < n && a[i] > a[i + 1]`. In every turn he will delete all the decreasing numbers at the same time. And he will repeat the process until there are no elements to delete. Now he asks you what the final sequence is.

### Input

The first line of input contains an integer `T (1 <= T <= 10)` which is the total number of test cases.

For each test case, there are two lines.

The first line contains a integer `n`, `n <= 100,000`.

The second line contains `n` integers, representing the sequence `{a[i]}` `(1 <= ai <= 100,000)`.

### Output

One line per test case, represents the final sequence after Peggy deletes.

### Sample Input
```
2
10
1 4 4 3 2 8 9 4 5 7
5
1 8 6 2 4
```

### Sample Output
```
1 4 7
1 4
```
