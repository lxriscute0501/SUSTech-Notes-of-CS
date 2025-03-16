# CS208 Theory Assignment 1

12312110 李轩然

**DDL: Mar.11 20:50**

## Chapter 1 Exercise 1

### Description

Decide whether you think the following statement is true or false. If it is true, give a short explanation. If it is false, give a counterexample.

*True or false? In every instance of the Stable Matching Problem, there is a stable matching containing a pair ``(m, w)`` such that m is ranked first on the preference list of ``w`` and ``w`` is ranked first on the preference list of ``m``.*

### Analysis

**False.**

**Counterexample:** Consider we have two men ``m = [m1, m2]`` and two women ``w = [w1, w2]``, and the preference profiles are as follows:

|Men|1st|2nd|
|:--:|:--:|:--:|
|m1|w2|w1|
|m2|w1|w2|

|Women|1st|2nd|
|:--:|:--:|:--:|
|w1|m1|m2|
|w2|m2|m1|

According to the stable matching algorithm, we take ``(m1, w2)`` first, keep going on men's preference profile, take ``(m2, w1)``. There is no rejection according to women's preference profile, thus 
```math
(m1, w2), 
(m2, w1)
```
is a prefect matching. However, there **does not** exsit a pair ``(m, w)`` such that ``m`` is ranked first on the preference list of ``w`` and ``w`` is ranked first on the preference list of ``m``.

<br>

## Chapter 1 Exercise 2

### Description

Decide whether you think the following statement is true or false. If it is true, give a short explanation. If it is false, give a counterexample.

*True or false? Consider an instance of the Stable Matching Problem in which there exists a man ``m`` and a woman ``w`` such that ``m`` is ranked first on the preference list of ``w`` and ``w`` is ranked first on the preference list of ``m``. Then in every stable matching ``S`` for this instance, the pair ``(m, w)`` belongs to ``S``.*

### Analysis

**True.**

First, consider ``m`` and ``w`` are first on each other's preference list, obviously it is stable, thus the pair ``(m, w)`` **can** exist in ``S``.

Second, we suppose that ``m`` is free when matching from ``m``'s preference list. Since ``w`` is ranked first on ``m``'s preference list, ``m`` chooses ``w``. Similarly, if ``w`` is free, ``m`` and ``w`` become engaged.

Third, we suppose that there exists a counterexample: ``(m, w')`` and ``(m', w)`` has matched as a temporary perfect matching. Then, ``m'`` must be behind ``m`` on the women's preference list, and ``w'`` must be behind ``w`` on the men's preference list, thus ``(m, w')`` and ``(m', w)`` is unstable, for ``(m, w)`` is a perfect matching. 

Thus, these cases provide the statement is true.


<br>

## Chapter 1 Exercise 3

### Description

There are many other settings in which we can ask questions related to some type of “stability” principle. Here’s one, involving competition between two enterprises.

Suppose we have two television networks, whom we’ll call ``A`` and ``B``. There are ``n`` prime-time programming slots, and each network has ``n`` TV shows. Each network wants to devise a schedule — an assignment of each show to a distinct slot — so as to attract as much market share as possible.

Here is the way we determine how well the two networks perform relative to each other, given their schedules. Each show has a fixed rating, which is based on the number of people who watched it last year; we’ll assume that no two shows have exactly the same rating. A network wins a given time slot if the show that it schedules for the time slot has a larger rating than the show the other network schedules for that time slot. The goal of each network is to win as many time slots as possible.

Suppose in the opening week of the fall season, Network ``A`` reveals a schedule ``S`` and Network ``B`` reveals a schedule ``T``. On the basis of this pair of schedules, each network wins certain time slots, according to the rule above. We’ll say that the pair of schedules ``(S, T)`` is **stable** if neither network can unilaterally change its own schedule and win more time slots. That is, there is no schedule ``S'`` such that Network ``A`` wins more slots with the pair ``(S', T)`` than it did with the pair ``(S, T)``; and symmetrically, there is no schedule `` T'`` such that Network ``B`` wins more slots with the pair ``(S, T')`` than it did with the pair ``(S, T)``.

The analogue of Gale and Shapley’s question for this kind of stability is the following: For every set of TV shows and ratings, is there always a stable pair of schedules? Resolve this question by doing one of the following two things:

&emsp; (a) give an algorithm that, for any set of TV shows and associated ratings, produces a stable pair of schedules; or
&emsp; (b) give an example of a set of TV shows and associated ratings for
which there is no stable pair of schedules.

### Analysis

**There is not always a stable pair of schedules, so we just do (b).**

Suppose we have two networks ``A, B``, and Network ``A`` has two shows ``[a1, a2]``, Network ``B`` has two shows ``[b1, b2]``. Their ratings are as follows:
|Show|Rating|
|:--:|:--:|
|a1|2|
|a2|4|
|b1|1|
|b2|3|

**Case 1:** ``a1`` is paired against ``b1``, then Network ``B`` is willing to swap ``b1`` and ``b2`` in its schedule. Thus, Network ``B`` will win at least one slot, which means this schedule is not stable. 

**Case 2:** ``a1`` is paired against ``b2``, then Network ``A`` is willing to swap ``a1`` and ``a2`` in its schedule. Then, Network ``B`` will win no slot, which means that it is willing to swap ``b1`` and ``b2``, then back to the original case. Thus, this schedule is also not stable.

As a result, according to these two cases, there does not exist a stable pair of schedules in this example.

<br>

## Chapter 1 Exercise 8 (optional)

### Description

For this problem, we will explore the issue of truthfulness in the Stable Matching Problem and specifically in the Gale-Shapley algorithm. The basic question is: Can a man or a woman end up better off by lying about his or her preferences? More concretely, we suppose each participant has a true preference order. Now consider a woman ``w``. Suppose ``w`` prefers man ``m`` to ``m'``, but both ``m`` and ``m'`` are low on her list of preferences. Can it be the case that by switching the order of ``m`` and ``m'`` on her list of preferences (i.e., by falsely claiming that she prefers ``m'`` to ``m``) and running the algorithm with this false preference list, ``w`` will end up with a man ``m''`` that she truly prefers to both ``m`` and ``m'``? (We can ask the same question for men, but will focus on the case of women for purposes of this question.)

Resolve this question by doing one of the following two things:

&emsp; (a) Give a proof that, for any set of preference lists, switching the order of a pair on the list cannot improve a woman’s partner in the Gale-Shapley algorithm; or
&emsp; (b) Give an example of a set of preference lists for which there is a switch that would improve the partner of a woman who switched preferences.

### Analysis

**Switching women's preferences can improve their partner, so we just do (b).**

Consider we have three men called ``m1, m2, m3`` and three women called ``w1, w2, w3``, and their preference profiles are as follows:

|Men|1st|2nd|3rd|
|:--:|:--:|:--:|:--:|
|m1|w1|w3|w2|
|m2|w3|w1|w2|
|m3|w1|w3|w2|

|Women|1st|2nd|3rd|
|:--:|:--:|:--:|:--:|
|w1|m2|m1|m3|
|w2|m1|m2|m3|
|w3|m1|m2|m3|

Suppose that woman ``w1`` switch her preferences list falsely, where she prefer ``m3`` to ``m1``:

|Woman|1st|2nd|3rd|
|:--:|:--:|:--:|:--:|
|w1|m2|m3|m1|

**Original case:** By Gale-Shapley algorithm, ``m1`` will choose ``w1``, then ``m2`` will choose ``w3``, and for ``w1`` and ``w3`` have been chosen, meanwhile ``m3`` has the least preference of ``w1`` and ``w3``, thus ``m3`` will choose ``w2``. So the stable matching will be:
```math
(m1, w1), 
(m2, w3), 
(m3, w2)
```

**Switched case:** 
1. ``m1`` will still choose ``w1``, and ``m2`` will still choose ``w3`` at first. However, for ``w1`` has the best preference of ``m3`` and ``m3`` is better than ``m1`` in the preference of ``w1``, ``w1`` will change to choose ``m3``, meanwhile ``m1`` will tend to choose ``w3``. 
2. But for ``w3``, ``m1`` has better preference than ``m2``, thus ``w3`` will change to choose ``m1``, and ``m2`` will tend to choose the 2nd preference ``w1``.
3. For ``w1``, ``m2`` has better preference than ``m3``, thus ``w1`` will change to choose ``m2``, thus ``m3`` has no choice but ``w2``.
4. So the new matching will be:
```math
(m1, w3),
(m2, w1),
(m3, w2)
```
Obviously, after switching ``m1`` and ``m3`` in ``w1``'s preferences list, ``w1`` matches ``m2`` that she prefers to both ``m1`` and ``m3``, so we are done.

