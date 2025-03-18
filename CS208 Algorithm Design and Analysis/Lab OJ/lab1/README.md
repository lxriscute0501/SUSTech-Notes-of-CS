# Lab1 A. File_System

## Description

In this task, you are required to implement a set of simple UNIX commands. There will be $n$ create/update commands and $m$ query commands. **This task involves only two two types of file system objects: file and folder.**

Initially, **there are no files or folders**, and you can consider that you have just **stepped into an empty directory** (the “initial directory”) and opened the terminal there.

Below is a detailed introduction to all commands:

### Create/Update Command

1. `echo [content] > [path_to_file]`

   This command create a **file** with content `[content]` at the specified `path_to_file`. The `content` only consists of letters $(a-z, A-Z)$ and $0-9$ digits; if `[content]` is empty, an empty **file** is created.

   Note: The path may contain `.` and `..`  in this task for all commands.

   `.` denotes the current directory

   `..` denotes the parent directory. (Since you start in the initial directory, no command will attempt to access a non-existent parent directory.)
2. `mkdir [path_to_folder]`

   This command creates a **folder** at the specified `[path_to_folder]`.
   For example:

   * `mkdir a` creates a folder named `a` in the initial directory.
   * `mkdir a/b` creates a folder named `b` inside folder `a`.
3. `rm [path_to_file]`

   This command removes the file located at **file** on `[path_to_file]`.
4. `rm -rf [path_to_folder]`
   This command recursively deletes the folder `path_to_folder` along with all files and subfolders within it, without any warning or confirmation.
5. `mv [src_path] [dst_path_to_folder]`
   This command moves the file or folder from `[src_path]` to the destination folder `[dst_path_to_folder]`. For example, `mv a b` moves the object `a` into folder `b`, resulting in the new path `b/a`.

### Query Command

1. `cat [path_to_file]`

   This command outputs the content of the file located at `[path_to_file]`.

2. `find [path] [expression]`

   This command find all files and folders starting from `[path]` and satisfy the given `[expression]`.
   The `[expression]` can include one or both of the following parts:

   * `-type f/d`: Specifies whether to search for files (`f`) or directories (`d`).
   * `-name [name]`: Specifies that the file or folder name must exactly match `[name]`.

   The `[path]` argument can be empty, which means the search starts from the initial directory (`.`).
   The `[expression]` can be empty, which means find all files and folders starting from `[path]`.
   If both `[path]` and `[expression]` are empty, the command should output all file and folder paths starting from the initial directory (`.`).
   For a `find` command, first output the total number of matching paths, then output each matching path on a new line. The order of the output paths does not matter.

All commands provided in the input are valid, and all file or folder names are unique within the same directory (including the initial directory).

## Input format

The first line contains two integers $n, m$.

The following $n$ lines denote the create or update commands.

Then subsequent $m$ lines denote the query commands.

## Output format

For each `cat` command, one line to output the content.

For each `find` command, the first line output the total number of matching paths, followed by each matching path on a separate line (order does not matter).

## Sample

### Sample Input
```
16 6
mkdir a
echo 123 > a/b
echo 234 > a/c
echo 345 > a/d
mkdir b
mkdir c
mkdir d
echo 666 > b/a
echo 23333 > c/a
echo 12312dasdasdf > d/a
mkdir a/e
echo > a/e/b
echo > b/b
echo > d/b
rm a/c
rm -rf a/e
cat d/a
cat c/a
cat d/b
find
find a -name b
find ././././ -name b -type f
```

### Sample Output
```
12312dasdasdf
23333

12
.
./a
./a/b
./a/d
./b
./b/a
./b/b
./c
./c/a
./d
./d/a
./d/b
1
a/b
3
././././a/b
././././b/b
././././d/b

```

You can use your own UNIX system to run the command to check the result.
**Note:** The test data will not include any cases that cause command execution to fail, so you do not need to handle such failures.


## Constraints:

For 100% testcases, $1 \le n \leq 550, 1 \le m \leq 10$. The length of content and path will not exceed $50$.

For 30% testcases, there are only `echo` operations for the $n$ commands.

For 60% testcases, there are not update commands.

<br>

# Lab1 B. Counting

## Description

Given an undirected connected graph $G$ with $n$ nodes and $m$ edges. Nodes are numbered starting from $1$ to $n$.

Given two integers $a, b$. Now counting the pairs $(x,y)$ that any path from node $x$ to node $y$ goes through node $a$ and node $b$ $(x \neq a, x \neq b, y \neq a, y \neq b, x \neq y)$.

Print the required number of pairs. The order of two nodes in a pair does not matter, that is, the pairs $(x,y)$ and $(y,x)$ must be taken into account only once.

## Input Format

The first line contains four integers, $n, m, a, b$.

Then following $m$ lines, each line contains two integer $u,v$ denoting that there is an undirected edge between node $u$ and node $v$.

## Output Format

A single line containing the total number of required pairs.

## Sample

### Sample Input1
```
7 7 3 5
5 6
6 7
7 5
1 2
2 3
3 4
4 5
```

### Sample Output1
```
4
```

### Sample Input2
```
6 6 2 3
1 2
2 3
3 4
4 6
5 6
1 5
```

### Sample Output2
```
0
```

## Constraits

For 50% testcases: $m = n - 1$.

For 100% testcases: $4 \leq n \leq 1 \times 10^5, n - 1\leq m \leq 2 \times 10^5, a,b \in [1, n]$.

