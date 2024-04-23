### List all C and H files in the current directory
```shell
ls -l | sequencia -s 'split nl
ranges [1, -1]
map
  split " "
  select 8
filter
  transform
    contains ".c"
  map 0
    contains ".h"
  any
join nl'
```

