### List all files and output date+time and file name seperated by tabs
```shell
ls -l | sequencia -s 'split nl
ranges [1, -1]
map
  split " "
  make 2
  map 0
    ranges [5, 7]
    join " "
  map 1
    select 8
  join tab
join nl'
```

