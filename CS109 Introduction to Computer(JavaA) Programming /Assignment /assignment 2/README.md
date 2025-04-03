# Assignment 2 - CS109 2025Spring

<br>

## Problem 1. Missing Records - 20 pts

### Description

Qi is currently engaged in an off-campus internship, responsible for recording the number of stray cats and dogs in different districts.
However, some records are missing. Please help her count how many records are missing in the specified district.

### Input

The input consists of 4 lines.

The first line contains a positive integer $n (1 \leq n \leq 50)$, representing the number of districts. Each district has a different number, starting from 1 and incrementing by 1.

The second line contains $n$ integers, representing the number of stray cats in each district, with each value separated by a space. If the record for a district is missing, it is represented by `-1`.

The third line contains $n$ integers, representing the number of stray dogs in each district, with each value separated by a space. If the record for a district is missing, it is represented by `-1`.

The fourth line contains a positive integer $m$, representing the number of the district to query.

### Output

Output an integer $num$, representing the number of missing records in the specified district.

### Sample 1

#### Sample Input
```
5
12 45 37 -1 95
-1 39 62 -1 0
1
```

#### Sample Output
```
1
```

#### Explanation

The first line has the number `5`, indicating that there are 5 districts in total. The district numbers are `1, 2, 3, 4`, and `5`. (Note: it is not `0, 1, 2, 3, 4`)

The second line contains the number of stray cats in each district, which are `12, 45, 37, missing`, and `95`.

The third line contains the number of stray dogs in each district, which are `missing, 39, 62, missing`, and `0`.

The fourth line indicates that the queried district is number `1`.

The stray dog data for district 1 is missing, so the output is `1`.


### Sample 2

#### Sample Input
```
5
12 45 37 -1 95
-1 39 62 -1 0
2
```

#### Sample Output
```
0
```

#### Explanation

The fourth line indicates that the queried district is number `2`.

In district 2, no data is missing, so the output is `0`.

<br>

## Problem 2. Data Repair - 30 pts

### Description

After counting the missing records, Qi decided to try and repair these missing records. She believes that the number of stray cats or dogs in each district may be related to the numbers in adjacent districts.

Specifically, if a district’s record is missing (i.e., the value is `-1`), its stray cat or dog number can be repaired by the **weighted average** of its neighboring districts.

The specific repair process for stray cats is as follows:

If both the left and right neighboring districts have complete records, the stray cat number for this district is the integer part of
```math
0.4 * Left-district-stray-catnumber + 0.6 * Right-district-stray-cat-number
```

**Note:** Take the **integer part**, not rounded.

If either the left or right neighboring district has a missing record, the stray cat number cannot be repaired and remains `-1`.

If the district is at the boundary (i.e., there is no left or right neighbor), the stray cat number is the integer part of
```math
0.8 * Left-or-Right-district-stray-cat-number
```

The repair process for stray dogs follows the same logic as for stray cats.

### Input

The input consists of 4 lines.

The first line contains a positive integer $n (1 \leq n \leq 50)$, representing the number of districts. Each district has a different number, starting from 1 and incrementing by 1.

The second line contains $n$ integers, representing the number of stray cats in each district, with each value separated by a space. If the record for a district is missing, it is represented by `-1`.

The third line contains $n$ integers, representing the number of stray dogs in each district, with each value separated by a space. If the record for a district is missing, it is represented by `-1`.

The fourth line contains a positive integer $m$, representing the district number to query.

### Output

Output a string $str$, representing the new stray cat and dog data in the queried district after repair.

The output should fall into one of the following cases:

If both stray cat and dog data are complete, output the total number of stray cats and dogs;     
If both stray cat and dog data are missing, output `ALL DATA LOST`;       
If stray cat data is missing and stray dog data is complete, output `CAT DATA LOST`;      
If stray cat data is complete and stray dog data is missing, output `DOG DATA LOST`.     

**Note:** All letters should be in uppercase, with a single space between words, and no punctuation at the end.

### Sample 1

#### Sample Input
```
7
12 45 37 -1 95 30 -1
-1 39 62 -1 -1 0  78
1
```

#### Sample Output
```
43
```

#### Explanation

The first line shows that there are 7 districts in total. The district numbers are `1, 2, 3, 4, 5, 6`, and `7`.

The second line contains the number of stray cats in each district: `12, 45, 37, missing, 95, 30`, and `missing`.

The third line contains the number of stray dogs in each district: `missing, 39, 62, missing, missing, 0`, and `78`.

After data repair, the new records are as follows:

Stray Cats: `12 45 37 71 95 30 24`

- For district 4, the stray cat data is repaired using the formula: $0.4 * 37 + 0.6 * 95 = 71.8$, and the integer part is `71`.
- For district 7, the stray cat data is repaired using the formula: $0.8 * 30 = 24.0$, and the integer part is `24`.
  
Stray Dogs: `31 39 62 -1 -1 0 78`

- For district 1, the stray dog data is repaired using the formula: $0.8 * 39 = 31.2$, and the integer part is `31`.
- For districts 4 and 5, the data cannot be repaired because their adjacent districts contain missing records, so the value remains `-1`.

The fourth line indicates the queried district is number 1. The data for district 1 has been repaired, and no data is missing, so the output is the total of stray cats and dogs, which is `43`.

### Sample 2

#### Sample Input 
```
7
12 45 37 -1 95 30 -1
-1 39 62 -1 -1 0  78
4
```

#### Sample Output
```
DOG DATA LOST
```

#### Explanation

After data repair, the new records are as follows:

Stray Cats: `12 45 37 71 95 30 24`

Stray Dogs: `31 39 62 -1 -1 0 78`

The fourth line indicates the queried district is number 4.

The stray cat data for district 4 has been repaired, but the stray dog data has not been repaired because the right neighboring record is missing. Therefore, the output is `DOG DATA LOST`.


<br>

## Problem 3. Two-Dimensional Data Recovery - 50 pts

### Description

After one round of recovery, Qi finds that some of the data in certain districts is still missing. Therefore, she decides to continue recovering the data for the stray cats. She needs to use historical data for a more complex recovery process.

Specifically, if a district's record is missing (i.e., its value is -1), the number of stray cats in that district can be recovered by the weighted average of its neighboring districts and neighboring historical data.

The formula for calculating the weighted average of the stray cats in a district is:
```math
(Left-coefficient * Left-district's-stray-cat-count + Right-coefficient * Right-district's-stray-cat-count + Up-coefficient * Data-from-the-previous-month + Down-coefficient * Data-from-the-next-month)
```

**Note:** Take the **integer part**, not rounded.

The coefficients are determined according to the following conditions:

1. If the data from the left neighboring district, right neighboring district, the previous month's historical data, and the next month's historical data are all available:

| Left Coefficient | Right Coefficient | Up Coefficient | Down Coefficient |
|:--:|:--:|:--:|:--:|
|0.3|0.3|0.2|0.2|

2. If either the left or right neighboring district has missing data or is non-existent, but the previous month's and next month's historical data are both available, the coefficient of the left or right neighboring district is evenly distributed between the historical data:

| Left / Right Coefficient | Up Coefficient | Down Coefficient |
|:--:|:--:|:--:|
| 0.3 | 0.2 + (0.3/2) =0.35 | 0.2 + (0.3/2) =0.35 |

3. If the data from the left and right neighboring districts are complete, but either the previous or next month's historical data is missing, the missing coefficient is evenly distributed between the district data:

| Left Coefficient | Right Coefficient | Up / Down Coefficient |
|:--:|:--:|:--:|
|0.3 + (0.2/2) =0.4 |0.3 + (0.2/2) =0.4 |0.2|

4. If either the left or right neighboring district 's data is missing, and either the previous or next month's historical data is missing, the following coefficients are used:

| Left / Right Coefficient | Up / Down Coefficient |
|:--:|:--:|
|0.6|0.4|

5. If both the left and right neighboring districts' data are missing or non-existent, but the previous and next month's historical data are available, the coefficients are as follows:

| Up Coefficient | Down Coefficient |
|:--:|:--:|
|0.5|0.5|

6. If the left and right neighboring districts' data are complete, but both the previous and next month's historical data are missing, the coefficients are:

| Left Coefficient | Right Coefficient |
|:--:|:--:|
|0.4|0.6|

7. If three or more of the left, right, previous month, and next month data are missing or non-existent, the data cannot be recovered. Return `DATA CANNOT BE REPAIRED`.


### Input

The input consists of several lines.

The first line contains an integer $m (1 \leq m \leq 12)$, which indicates the number of months of historical data. The months are numbered from $1$ to $m$.

The second line contains an integer $n (1 \leq n \leq 50)$, which indicates the number of districts. Each district has an unique ID number, starting from 1 and incrementing by 1.

The next $m$ lines contain the historical data for the stray cats for each month. Each line contains $n$ integers separated by spaces. If the data for a district is missing, it is represented by `-1`.

The last line contains two positive integers: the first integer, $month$, represents the month being queried; the second integer, $district$, represents the district ID being queried.


### Output

Output a single integer $num$, which is the number of stray cats in the specified district for the given month. It is important to note that if the query data **is not** `-1`, the query data is printed directly; otherwise, the repaired data is printed. If the data cannot be recovered, print `DATA CANNOT BE REPAIRED`.


### Sample 1

#### Sample Input
```
3
7
12 45 -1 37 95 30 -1
-1 -1 62 -1 36 0  -1
24 61 79 49 77 2  -1
2 4
```

#### Sample Output
```
46
```

#### Explanation

The first line shows that there are 3 months of historical data.

The second line shows that there are 7 districts, numbered from 1 to 7.

The following 3 lines
```
12 45 −1 37 95 30 −1
−1 −1 62 −1 36 0 −1
24 61 79 49 77 2 −1
```

show the data for the stray cats across the three months. Missing data is represented by `-1`.

The last line indicates the query for month 2 and district 4. The data for district 4 in month 2 is missing and needs to be recovered.

To recover the data, we use the coefficients based on the availability of neighboring district data and historical data. In this case, the coefficients are `0.3` for the left district, `0.3` for the right district, `0.2` for the previous month, and `0.2` for the next month.

The weighted sum is calculated as: $0.3 * 62 + 0.3 * 36 + 0.2 * 37 + 0.2 * 49 = 46.6$

Taking the integer part gives a result of **46**, which is the recovered value for district 4 in month 2.

Thus, the output is `46`.

### Sample 2

#### Sample Input
```
3
7
12 45 -1 37 95 30 -1
-1 -1 62 -1 36 0  -1
24 61 79 49 77 2  -1
2 1
```

#### Sample Output
```
18
```

#### Explanation

The last line indicates the query for month 2 and district 1. The data for district 1 in month 2 is missing (the value is `-1`) and needs to be recovered.

To recover the data, we use the coefficients for the left and right neighboring district, the left district's data is non-existent and the right district's data is missing. However, the historical data for the previous and next months are available. The coefficients are `0.5` for both the previous and next months.

The weighted sum is: $0.5 * 12 + 0.5 * 24 = 18$

Thus, the output is `18`.

### Sample 3

#### Sample Input
```
3
7
12 45 -1 37 95 30 -1
-1 -1 62 -1 36 0  -1
24 61 79 49 77 2  -1
2 2
```

#### Sample Output
```
55
```

#### Explanation

The query is for month 2 and district 2. The data for district 2 in month 2 is missing (the value is `-1`), and we need to recover it.

The coefficients are as follows:

- Left district data is missing.
- Right district data is available.
- The historical data for the previous and next months are both available.

The coefficients for the left, right, and historical data are: `0.3` for the left district, `0.35` for the right district, and `0.35` for the historical data.

The weighted sum is: $0.3 * 62 + 0.35 * 45 + 0.35 * 61 = 55.7$

Taking the integer part gives a result of **55**.

Thus, the output is `55`.

### Sample 4

#### Sample Input
```
3
3
11 12 13
18 99 41
17 16 15
2 2
```

#### Sample Output
```
99
```

#### Explanation

The query is for month 2 and district 2. The data for district 2 in month 2 exists.

Thus, the output is directly `99`.

### Sample 5

#### Sample Input
```
1
2
1 -1
1 2
```

#### Sample Output
```
DATA CANNOT BE REPAIRED
```

#### Explanation

The query is for month 1 and district 2. The data for district 2 in month 1 is missing. However, we don't have enough data to recover it.

Thus, the output is `DATA CANNOT BE REPAIRED`.

**Note:** The recovery process only requires one iteration, so we do not need to consider multiple iterations of recovery.

Specifically, for Sample 3, the data for district 2 in February is missing. The data for the left neighboring district is `-1`, but referring to the explanation in Sample 2, we know that the data for the left neighboring district can be recovered.

Therefore, recovering the data for district 2 in February can be considered in the following two ways:

1. First, recover the data for district 1 in February, and then recover the data for district 2 in February. This would be two iterations.
2. Only recover the data for district 2 in February. This would be one iteration.

In our problem, we only consider **one iteration** of recovery.
