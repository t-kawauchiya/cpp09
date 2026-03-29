## what is merge insertion sort

- Ford-Johnson algorithm
- fewer comparisons in the worst case than other algorithms.
-

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

### about the order

- the order is for using efficient binary insertion.
- the binary insertion is effecient most when the list size is $2^k-1$.
  - the binary search is effecient most when candidate are $2^k$.

- lets compare bad order insertion and correct order insertion.

#### bad order insertion

- (correct insertion order is {y4,y3}, this needs 4 comparisons in worst case)

```
x1-x2-x3-x4
      |  |
      y3 y4
```

- insert y3 before insertion of y4.
  - insertion of y3 needs 2 comparisons in worst case.
  - the size of the list{x1-x3} is 3, that is $2^k-1$.

```
x1-y3-x2-x3-x4
            |
            y4
```

- insert y4 needs 3 comparisons in worst case to insert the list{x1-x3}.
- the size of the list{x1-x3} is 4, that is not $2^k-1$.

so this needs 5 comparisons in worst case.

#### correct order insertion

```
x1-x2-x3-x4
      |  |
      y3 y4
```

- insert y4.
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

that is why we use Jacbsthal numbers that guarantees efficient binary insertion.

