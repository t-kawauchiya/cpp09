## what is merge insertion sort

- Ford-Johnson algorithm
- fewer comparisons in the worst case than other algorithms.

### algorithm

1. group the X elemnts into $\lfloor n/2 \rfloor$ pairs and compare each pairs.

```
o  o  o  o  o  o  o  o ... (largers)
|  |  |  |  |  |  |  |
o  o  o  o  o  o  o  o ... (smallers)  o (for odd n, one item remain.)
```

2. sort largers using merge insertion sort recursively.

```
x1-x2-x3-x4-x5-x6-x7-x8-..-x(n/2+1)
      |  |  |  |  |  |
      y3 y4 y5 y6 y7 y8 .. y(n/2+1)  (y(n/2+2) : for odd n)
```

3. insert yi with a specially chosen order using binary insertion.

- the order is y4-3(2 items), y6-5(2), y12-7(6), y22-43(22) ...
  the item number of each groups are 2\*Ji(Jacobsthal numbers)
- process unpaired item when other items are all inserted

### why we use Jacbsthal numbers to insert

- to garuntee efficient binary insertion order we should use Jacbsthal numbers to detremine the index insertion starts.

- the binary insertion is most effecient when the list size is $2^k-1$.
  - the binary search is effecient most when candidate are $2^k$.
- insertion using Jacobsthal numbers garantees the list size is $2^k - 1$ .

### comparison between bad order insertion and correct order insertion

- first we compare bad order insertion and correct order insertion.

#### bad order insertion

- (correct insertion order is {y4,y3}, this needs 4 comparisons in worst case)
- this needs 5 comparisons in worst case.

```
x1-x2-x3-x4
      |  |
      y3 y4
```

- insertion of y3 needs 2 comparisons in worst case.
  - the size of the list{x1-x3} is 3, that is $2^k-1$.

```
x1-y3-x2-x3-x4
            |
            y4
```

- insert y4 needs 3 comparisons in worst case to insert the list{x1-x3}.
  - the size of the list{x1-x3} is 4, that is not $2^k-1$.

- so this needs 5 comparisons in worst case.

#### correct order insertion

this needs only 4 comparisons in worst case!!

```
x1-x2-x3-x4
      |  |
      y3 y4
```

- insertion of y4 needs 2 comparisons in worst case.
  - the size of the list{x1-x3} is 3, that is $2^k-1$.

```
x1-y4-x2-x3-x4
         |
         y3
```

- insert y4 needs 2 comparisons in worst case to insert the list{x1-x2}.
  - the size of the list{x1-x2} is 3, that is $2^k-1$.

so this needs 4 comparisons in worst case!!

#### what is Jacobsthal numbers

the definetion of Jacobsthal number is below

$$
J_0 = 0,\quad J_1=1
$$

$$
J_n = J_{n-1} + 2 J_{n-2} \\
or \\
J_n + J_{n+1} =  2^n \\
$$

$$\{J_n\} = \{0,1,1,3,5,11,21,...\}$$

#### sumation of Jacobsthal numbers

$$
\begin{aligned}
\sum^n J_n &=J_{n-1} + 2 J_{n-2} \\
&\quad+J_{n-2} + 2 J_{n-3} \\
&\quad... \\
&\quad+J_2 + 2 J_1 \\
&\quad+J_1 \\
&\quad+J_1 \\
&=J_{n-1} + 3\sum^{n-2} J_n + J_1 \\
&=J_{n-1} + 3\sum^{n} J_n - 3\lparen J_n+ J_{n-1}\rparen + J_1 \\
&= \frac{1}{2}\{ - J_{n-1} + 3\lparen J_n + J_{n-1}\rparen - J_1\} \\
&= \frac{1}{2}\{ J_{n} + 2\lparen J_n+ J_{n-1}\rparen - J_1\} \\
&= \frac{1}{2}\lparen J_n + 2^{n}- 1 \rparen \\
\end{aligned}
$$

#### chain size

```
x1-x2-x3-x4-x5-x6-x7-x8-..-x(n/2+1)
      |  |  |  |  |  |
      y3 y4 y5 y6 y7 y8 .. y(n/2+1)  (y(n/2+2) : for odd n)
```

when we insert y4 the insertion target list must be {x1,x2,x3}. the size is sum of 2 elements at head(x1,x2) and pushed high elements(x3. that is 2\*J_1-1).

$$
size_1 = 2 + 2J_1 -1 = 3 = 2^{2} -1
$$

```
y4-x1-x2-y3-x3-x4-x5-x6-x7-x8-..-x(n/2+1)
                  |  |  |  |
                  y5 y6 y7 y8 .. y(n/2+1)  (y(n/2+2) : for odd n)
```

when we insert y6 the insertion target list must be like {y4,x1,x2,y3,x3,x4,x5}.
the size of list, size2 is sum of size1(3:x1,x2,x3), processde low elements(y3,y4) and pushed high elements(x4, x5. that is 2\*J_2)

$$
\begin{aligned}
size_2 &= size_1 + 2 J_1 +  2J_2  \\
 &= 2 + 4J_1 + 2J_2 -1 \\
 &= 7 = 2^{3} -1
\end{aligned}
$$

in general

$$
\begin{aligned}
size_n &= size_{n-1} + 2 J_{n-1} +  2J_n \\
 &= 2 + 4\sum J_{n-1} + 2J_n - 1 \\
 &= 1 + 4 * \frac{1}{2}\lparen J_{n-1} + 2^{n-1}- 1 \rparen + 2J_n \\
 &= 1 + 2 J_{n-1} + 2^{n}- 2 + 2J_n \\
 &= 2 \lparen J_n + J_{n-1}\rparen + 2^{n} - 1  \\
 &= 2 * 2^{n} - 1  \\
 &= 2^{n+1} -1
\end{aligned}
$$
