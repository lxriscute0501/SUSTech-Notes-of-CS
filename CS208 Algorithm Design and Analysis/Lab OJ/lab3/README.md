# Lab3 A. LRU Cache

## Description

You need to implement a **Least Recently Used (LRU) Cache** that supports the following operations efficiently:

1. `get key` - Print the value of the key if it exists; otherwise, return `-1`.
2. `put key value` - Update the value of the key if the key exists. Otherwise, add the key-value pair to the cache. If the cache exceeds its capacity after this operation, evict the **least recently used** key.

Both operations must run in **O(1) average time complexity**.

## Constraints

* The cache should be implemented without using `LinkedHashMap` in Java.

## Input

* The first line contains two integers `N` and `M` (1 ≤ N ≤ 1,000,000, 1 ≤ M ≤ 2,000,000) — the cache capacity and the number of operations.
* The next `M` lines contain one of the following operations:
  * `put key value` (1 ≤ key ≤ 1e9, value ≤ 1e9)
  * `get key` (1 ≤ key ≤ 1e9)

## Output

Print the output of each `get key` operation on a separate line.

## Example

### Input

```
2 9
put 1 1
put 2 2
get 1
put 3 3
get 2
put 4 4
get 1
get 3
get 4
```

### Output

```
1
-1
-1
3
4
```

## Notes

* If a key is retrieved using `get`, it becomes the **most recently used**.
* If a new key is inserted and the cache is full, the **least recently used** key is removed.
* The `put` operation updates the key if it already exists and **moves it to the most recently used position**.

<br>

# Lab3 B. Task Scheduler

## Description

You are given $n$ tasks and $m$ available time slots.\
Each task has a time interval $[l_i, r_i]$, and each time slot has an interval $[s_j, e_j]$.

A task can be scheduled **only if it is completely contained** within a single time slot, i.e.,  $l_i \ge s_j$ and $r_i \le e_j$. A time slot can be reused for multiple tasks.

Your goal is to **schedule as many tasks as possible**.

## Input

* The first line contains two integers $n$ and $m$ (1 ≤ n, m ≤ 100,000) — the number of tasks and time slots.
* The next $n$ lines each contain two integers $l_i$ and $r_i$ (1 ≤ $l_i$ < $r_i$ ≤ 1e9), representing a task.
* The next $m$ lines each contain two integers $s_j$ and $e_j$ (1 ≤ $s_j$ < $e_j$ ≤ 1e9), representing a time slot.

## Output

Print a single integer that is the maximum number of tasks that can be scheduled.

## Example

### Input

```
5 3
1 3
2 5
6 8
7 9
10 11
1 4
5 7
6 12
```

### Output

```
4
```

## Notes

* Tasks must be fully contained in one time slot.
* Time slots can be reused for multiple tasks.
