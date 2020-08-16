# Malloc lab RESULTS

Solution: use segregated free list to implement BEST-FIT.

With hard-coded strategy for binary1-bal.rep, binary2-bal.rep :

```
Results for mm malloc:
trace  valid  util     ops      secs  Kops
 0       yes   99%    5694  0.000310 18362
 1       yes  100%    5848  0.000275 21265
 2       yes   99%    6648  0.000359 18544
 3       yes  100%    5380  0.000269 20015
 4       yes   98%   14400  0.000339 42428
 5       yes   96%    4800  0.000526  9122
 6       yes   95%    4800  0.000563  8518
 7       yes   97%    6000  0.003398  1766
 8       yes   90%    7200  0.004601  1565
 9       yes  100%   14401  0.000300 48083
10       yes   87%   14401  0.000344 41839
Total          96%   89572  0.011284  7938

Perf index = 58 (util) + 40 (thru) = 98/100
```

Without hard-coded strategy:
```
Results for mm malloc:
trace  valid  util     ops      secs  Kops
 0       yes   99%    5694  0.000426 13382
 1       yes  100%    5848  0.000387 15127
 2       yes   99%    6648  0.000461 14424
 3       yes  100%    5380  0.000421 12782
 4       yes   98%   14400  0.000517 27842
 5       yes   96%    4800  0.000777  6174
 6       yes   95%    4800  0.000937  5124
 7       yes   55%    6000  0.000370 16216
 8       yes   51%    7200  0.000551 13072
 9       yes  100%   14401  0.000412 34971
10       yes   87%   14401  0.000422 34101
Total          89%   89572  0.005680 15769

Perf index = 53 (util) + 40 (thru) = 93/100
```

