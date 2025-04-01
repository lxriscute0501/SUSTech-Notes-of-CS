# Assignment 1 - CS109 2025Spring

<br>

## Problem 1. Treasure Map - 10 pts

### Description

Qi has obtained a treasure map. The map marks a total of `10` locations, numbered `0, 1, 2, 3, 4, 5, 6, 7, 8, 9`, but only one of these locations is the true treasure site. 
He needs to follow the map's hints to find the treasure location.

### Input

The input consists of several lines.

The first line is a positive integer $n (1 \leq n \leq 1000)$, indicating the number of test cases.

The next $n$ lines are the test cases.

Each test case occupies one line, and each test case is a positive integer $m$, representing the hint for the treasure location.

### Output

For each test case, output an integer $num$, representing the true treasure's number.

The number is the last digit of the integer $m$.

Then output a newline character.

After that, output the result for the next test case.

### Samples 1

#### Sample Input
```
2
99
1000
```

#### Sample Output
```
9
0
```

#### Explanation

The first line is `2`, indicating there are `2` test cases.

The first test case is `99`, and its last digit is `9`, so the output is `9`.

Then output a newline character.

The second test case is `1000`, and its last digit is `0`, so the output is `0`.


<br>

## Problem 2. Treasure Password - 20 pts

### Description

Qi has arrived at the treasure site, but the treasure is hidden in a secret cave. The cave entrance has a mechanism that only opens when the correct password is entered.
The password consists of `3` digits, and the order of the digits needs to be determined based on the hints.

### Input

The input consists of `2` lines.

The first line is a positive integer $n (100 \leq n \leq 999)$, representing the `3` digits used to form the password.

The second line is a positive integer $m$, which can be either `0` or `1`.

If $m$ is `0`, it means the digits in the password should be arranged in descending order;       
if $m$ is `1`, it means the digits should be arranged in ascending order.

### Output

Output a 3-digit integer $num$, representing the password.

### Sample 1

#### Sample Input
```
396
0
```

#### Sample Output
```
963
```

#### Explanation

The three digits are `3, 9, 6`. The second line is `0`, indicating that the digits should be arranged in descending order, so the password is `963`.


### Sample 2

#### Sample Input
```
396
1
```

#### Sample Output
```
369
```

#### Explanation

The three digits are `3, 9, 6`. The second line is `1`, indicating that the digits should be arranged in ascending order, so the password is `369`.

### Sample 3

#### Sample Input
```
505
1
```

#### Sample Output
```
055
```


<br>

## Problem 3. Treasure Box - 40 pts

### Description

Qi successfully opened the cave and found a pile of boxes inside. Each box has an unique number, and the numbers of boxes are consecutive integers.

There is a hint next to the boxes: only the box with number that is **perfect number** contains treasure.

**Perfect Number** refers to a positive integer that is equal to the sum of all its **proper divisors** (i.e., divisors excluding itself). For example, `6` is a perfect number because its proper divisors are `1, 2, 3`, and `1 + 2 + 3 = 6`.

Please help Qi find which box contains the treasure.

### Input

The input consists of only one line.

This line contains two positive integers $l$ and $r$, representing the smallest and largest numbers on the boxes, respectively. The two integers are separated by a space.

### Output

For each test case, output an integer $num$, representing the number of the box that contains the treasure.

If no box contains treasure, output `-1`.

In each test case, we guarantee that there will be no more than one perfect number.

### Sample 1

#### Sample Input
```
1 10
```

#### Sample Output
```
6
```

#### Explanation

In the range $[1, 10]$, there is one perfect number, `6`, so the output is `6`.

### Sample 2

#### Sample Input
```
1 2
```

#### Sample Output
```
-1
```

#### Explanation

In the range $[1, 2]$, there are no perfect numbers, so the output is `-1`.

<br>

## Problem 4. Finding the Exit - 30 pts

### Description

Qi successfully opened the treasure box, but inside the box was a mysterious note that read:

> "To safely leave the cave, you need to solve the mystery of the cave exit's temperature~~"

The cave has several exits, each with a different temperature. Qi needs to find the exit with the temperature closest to the target temperature to leave safely.

The temperature units of each exit vary: **Celsius (摄氏度), Fahrenheit (华氏度), and Kelvin (开尔文)**.

Please help Qi quickly convert the temperatures and leave the cave safely.

### Conversion Formulas

Celsius to Fahrenheit: $F = C * 9/5 + 32$

Fahrenheit to Celsius: $C = (F − 32) * 5/9$

Celsius to Kelvin: $K = C + 273.15$

Kelvin to Celsius: $C = K − 273.15$

### Input

The target temperature and each exit's temperature are represented in the following format: unit value. (There is a space between unit and value)

Unit can be one of 3 types: `C, F, K`, where `C` represents Celsius, `F` represents Fahrenheit, and `K` represents Kelvin.

For example, `C 30` represents `30` degrees Celsius.

The input consists of several lines.

The first line is the target temperature.

The second line is a positive integer $n$, representing the number of exits.

The next $n$ lines are the temperatures of the $n$ exits. Each exit's temperature occupies one line. Remember that temperatures are **float numbers**.

### Output

Output a positive integer, representing the number of the exit that can be used to safely leave the cave.

**Note:** Exit numbers start from `1` and increase by `1`.

### Sample 1

#### Sample Input
```
C 30
3
K 100
C 100
F 100
```

#### Sample Output
```
3
```

#### Explanation

The target temperature is `30` degrees Celsius.

Convert the temperature of exit 1 from Kelvin to Celsius, which is `-173.15` degrees Celsius, differing from the target temperature by `203.15` degrees Celsius.

The temperature of exit 2 is `100` degrees Celsius, differing from the target temperature by `70` degrees Celsius.

Convert the temperature of exit 3 from Fahrenheit to Celsius, which is approximately `37.78` degrees Celsius, differing from the target temperature by `7.78` degrees Celsius.

Exit 3 is the closest to the target temperature, so the output is `3`.


