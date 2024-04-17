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
