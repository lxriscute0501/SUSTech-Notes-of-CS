# Lab 10 A. Cookie Optimization at SUSTech

## Background

In 2025, the student-run "Xiaobaicai Store" at SUSTech launched a new series of cookies. To enjoy his favorite "Yilin Cookies" every day, student **smy** must please the merciless store manager **Xiaobai**. The store offers three types of cookies:

* **Yilin Cookies**: limited in stock
* **Regular Cookies**: unlimited in stock
* **Exclusive Cookies**: only one per subtype

Each cookie type has several subtypes. There are **m** subtypes in total. The *i-th* subtype has a price of **cᵢ**.\
However, Xiaobai insists that smy **must spend exactly n yuan** to check out.

Each subtype cookie has a specific **value $v_i$** (per cookie). Since smy is required to spend all **n yuan**, he hopes to maximize the **total value** of cookies he can purchase.

## Problem Description

Given $n$, $m$, and for each cookie subtype: category, quantity, price, and value — calculate the **maximum total value** smy can obtain.

## Input Format

* Line 1: Two integers $n$ and $m$ — total money to spend and number of cookie subtypes.
* The next $m$ lines describe each subtype:
  * $op_i = 0$: Regular Cookie — followed by two integers **$c_i, v_i$**
  * $op_i = 1$: Exclusive Cookie — followed by two integers **$c_i, v_i$**
  * $op_i = 2$: Yilin Cookie — followed by three integers **$x_i, c_i, v_i$**, where **$x_i$** is the stock of that subtype

## Output Format

Output a single integer: the **maximum total value** smy can get.

## Samples

### Sample Input 1

```
5 2
0 2 3
1 3 4
```

### Sample Output 1

```
7
```

### Explanation

Spend 2 yuan on the regular cookie (value 3), and 3 yuan on the exclusive cookie (value 4), totaling 7 in value.

### Sample Input 2

```
6 3
0 3 3
1 1 12
2 1 4 100
```

### Sample Output 2

```
6
```

### Explanation

Xiaobai insists that smy **must spend exactly n yuan** to check out.
So, smy spend 6 yuan on the regular cookie (cost `3*2`),  totaling 6 in value(value `3*2`).
If he buys a Yilin cookie (cost 4) or an Exclusive cookie (cost 1), he won’t be able to spend all his remaining money exactly.

### Sample Input 3

```
6 3
0 4 3
1 1 12
2 1 4 100
```

### Sample Output 3

```
0
```

### Explanation

There is no purchasing plan for smy that can both spend all the money exactly and meet the cookie purchase restrictions; in this case, output 0.

## Constraints

* For 40% of test cases: $0 < n \leq 100, \ 0 < m \leq 100$
* For 100% of test cases: $0 < n \leq 2000, \ 0 < m \leq 2000, \ c_i \leq 20, \ v_i \leq 10^5, x_i \leq 200$

<br>


# Lab 10B: Fan Dance Combinations

## Problem Description

In the SUSTech Traditional Culture Club, students are performing a **modern fan dance** inspired by Korean heritage. The dance begins with **n dancers**, each holding **one fan**. The entire performance lasts for **m seconds**.

During each second, dancers may pass some or all of their fans to the adjacent person (left or right), or keep them. They can also receive fans from adjacent neighbors. A fan received in the current second **cannot** be passed on again in the same second.

Each dancer may hold any number of fans, including none.

## Objective

Determine the number of **distinct final configurations** (i.e., how many fans each dancer ends up with), modulo **99824353**.

Two configurations are considered different if at least one dancer ends up with a different number of fans.

## Input Format

* A single line with two integers: $n$ (number of dancers), $m$ (duration in seconds)

## Output Format

* One integer: the number of possible configurations after m seconds, modulo **99824353**

## Samples

### Sample Input 1

```
3 5
```

### Sample Output 1

```
10
```

### Sample Input 2

```
100 5
```

### Sample Output 2

```
9447126
```

## Constraints

* For 10% of the data: $n, m \leq 3$
* For 40% of the data: $n, m \leq 10$
* For 100% of the data: $n, m \leq 100$
