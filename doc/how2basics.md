# Basic examples

### Parse `ls -l` output
```shell
ls -l | sequencia -s 'split nl
map
    split " "
    select [1, 2]
    join " "
join nl'
```

### List all C files in the current directory
```shell
ls -l | sequencia -s 'split nl
filter
  split " "
  select 8
  contains ".c"
map
  split " "
  select 8
join nl'
```

### Check if a string is all alphanumeric
```shell
echo 'abc123' | sequencia -s 'transform
  falnum
same'
```

### List all C and H files in the current directory
```shell
ls -l | sequencia -s 'split nl
filter
  split " "
  select 8

  make 2
  map 0
    contains ".c"
  map 1
    contains ".h"
  any

map
  split " "
  select 8
join nl'
```

### List all files and output date+time and file name seperated by tabs
```shell
ls -l | sequencia -s 'split nl
map
  split " "
  make 2
  map 0
    ranges [5, 7]
    join " "
  map 1
    select 8
  join tab
  noempty
join nl'
```

### List all files with date+time, file name and size
```shell
ls -l | sequencia -s 'split nl
map
  split " "
  select [5,6,7, 8, 8]
  noempty
map
  use [0,1]
    join " "
  use [0,1]
    join tab
  map 1
    prepend "du -b "
    with
      run
      split tab
      select 0
    split ""
    rev
    group 3
    map
      rev
      join ""
    rev
    join ","
    append tab
    append "B"
  join tab
join nl'
```

### Pretty print date
```shell
date | sequencia -s 'split " "
select [1, 2, 5, 3]
use [0, 1]
  rev
  join "th "
use [0, 1]
  join " "
join tab'
```

### (Stupid) Check if a string is all alphanumeric
Note: Comments are not yet implemented
```
split ""      # make each character separate string
map           # for every character (as string):
  make 3      #   create an array of 3 elements and fill with character (as string)
  map 1       #   modify the second element:
    fdigit    #     remove all chars from the string (char of main string) that are not a digit
  map 2       #   modify the third element:
    falpha    #     remove all chars from the string (char of main string) that are not a letter
  axis        #   [A, 0, 1, ..] -> [[A, 0], [A, 1], ...]
  map         #   go trough all checks (digit and alpha):
    same      #     is it the filtered char the same as the normal char
  any         #   any of these checks true
same          # all true
```
