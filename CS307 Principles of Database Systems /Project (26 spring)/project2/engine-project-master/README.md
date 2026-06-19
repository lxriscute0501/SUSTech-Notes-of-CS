# 数据库引擎开发项目介绍


## 项目介绍：

本次项目基于提供的**Java框架**（已实现部分核心模块），完成一个简易数据库引擎的开发，包括：SQL语句解析、逻辑算子、物理算子、存储层设计、数据转换等模块。需要同学们读懂框架，并根据已给出的内容与代码样例，进行补充完善，并实现相关类似的功能。 目前框架中已给出的功能如下：

- 磁盘I/O读写（存储层设计）
- `CREATE TABLE`、`INSERT`、`UPDATE` 语句支持
- 等值筛选（`WHERE`）与逻辑判断（`AND`）
- 全表扫描（`SELECT *` 及 `SeqScan` 操作）

## 项目搭建：

- 该项目基于```Maven```工程搭建，建议用过Intellij IDEA打开。 

- 成功导入maven所有依赖后，请在项目根目录，创建文件夹，命名为：```CS307-DB``` 最后项目层级如下显示：

  ```
  engine-project
  	- CS307-DB
  	- src
  	- pom.xml
  ```

## 使用项目：

运行程序后，可以输入下面的指令尝试使用项目：

**create table**

```sql
create table t( id int, name char, age int, gpa float);
```

**insert**

```sql
insert into t (id, name, age, gpa) values (1, 'a', 18, 3.6);
insert into t (id, name, age, gpa) values (2, 'b', 19, 3.65);
insert into t (id, name, age, gpa) values (3, 'abb', 18, 3.86);
insert into t (id, name, age, gpa) values (4, 'abc', 19, 2.34);
insert into t (id, name, age, gpa) values (5, 'ef', 20, 3.25);
insert into t (id, name, age, gpa) values (6, 'bbc', 21, 3.20);
```

**select**

```sql
select * from t;
select * from t where t.age = 19;
```



## 问题反馈：

如果读者有任何问题，可以提出issue。但由于一些**未能给出**的功能会用作教学要求中的任务，但在已实现的功能中，如若发现项目框架本身可能存在的bug，欢迎提出pull request。

