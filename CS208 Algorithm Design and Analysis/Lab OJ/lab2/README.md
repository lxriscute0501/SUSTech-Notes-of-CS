# Lab2 A. Food chain

## Description

> "Why do we need to learn biology in order to get into CS??"

> "I don't know\..."

There are $M$ predation relationships among $N$ species in some fauna. It is guaranteed that there is no cylic predation in the food web.

A food chain is defined by a sequence of species $[a_1, a_2, \dots, a_k]$, where

* nothing can prey on $a_1$
* $a_k$ can prey on nothing
* $a_i$ can prey on $a_{i+1}$ for $i = 1, 2, \dots, k-1$

Two food chains are different, if and only if their sequences are different.

Now each of these $N$ species wants to know how many food chains involve with it, module $10^9+7$.

## Input Format

The first line contains two integers $N, M$.

For the following $M$ lines, the $i^{th}$ line contains two integers $x_i, y_i$, meaning that species $x$ preys on species $y$.

## Output Format

Output one line with $N$ integers separated by spaces, representing the number of food chains involved with each species.

## Samples

### Sample 1 Input
```
5 6
4 5
4 3
2 4
3 1
2 3
5 1
```

### Sample 1 Output
```
3 3 2 2 1
```

### Sample 2 Input
```
8 8
2 4
7 5
3 4
3 6
2 6
7 8
2 3
5 1
```

### Sample 2 Output
```
1 4 2 2 1 2 2 1
```

## Limitations & Hints

For $20\%$ testcases:

* $N \le 10$

For another $10\%$ testcases:

* $M = N - 1$
* $x_i = i,\  y_i = i + 1$

For $100\%$ testcases:

* $1 \le N, M \le 10^5$
* $1 \le x_i, y_i \le N,\ x_i \neq y_i$

<br>

# Lab2 B. Legendary Grabbing Machine

## Description

Satori is a hunter who likes hunting bunnies.

Initially at moment 0, there are $N$ bunnies, the $i^{th}$ of which is at position $p_i$.

There are also $M$ nests, the $i^{th}$ of which is at position $q_i$. Each nest can hold at most $C$ bunnies.

Each bunny can move at most 1 unit of distance within 1 unit of time. Once a bunny enters some nest, it will be completely safe from Satori.

Satori's **Legendary Grabbing Machine** takes $T$ units of time to charge. Once the machine finishes charging at moment $T$, all bunnies that are out of nest will be captured. Note that bunnies entering nest at moment $T$ will be safe.

The bunnies very are united. They wish to know the maximum number of safe bunnies if they move optimally.

## Input Format

The first line contains 4 integers $N, M, C, T$.

The second line contains $N$ integers $p_1, p_2, \dots, p_N$.

The third line contains $M$ integers $q_1, q_2, \dots, q_M$.

## Output Format

One integer — the maximum number of bunnies who will not be captured by Satori.

## Samples

### Sample 1 Input
```
2 2 1 5
45 55
40 60
```

### Sample 1 Output
```
2
```

### Sample 2 Input
```
2 2 1 5
45 54
40 60
```

### Sample 2 Output
```
1
```

## Limitations & Hints

For $50\%$ of testcases:

* $1 \le N, M, C \le 2000$

For $100\%$ of testcases:

* $1 \le N, M, C \le 2 \times 10^5$
* $|T| \le 10^9$
* $|p_i|, |q_i| \le 10^9$


