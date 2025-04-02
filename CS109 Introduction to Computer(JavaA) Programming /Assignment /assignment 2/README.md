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
0.4 * Left district stray cat number + 0.6 * Right district stray cat number
```

**Note:** Take the **integer part**, not rounded.

If either the left or right neighboring district has a missing record, the stray cat number cannot be repaired and remains `-1`.

If the district is at the boundary (i.e., there is no left or right neighbor), the stray cat number is the integer part of
```math
0.8 * Left or Right district stray cat number
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

The fourth line indicates the queried district is number 1. The data for district 1 has been repaired, and no data is missing, so the output is the total of stray cats and dogs, which is 43.




