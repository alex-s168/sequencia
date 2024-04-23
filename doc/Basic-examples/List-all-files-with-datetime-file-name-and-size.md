### List all files with date+time, file name and size
```shell
ls -l | sequencia -s 'split nl
ranges [1, -1]
map
  split " "
  select [5,6,7, 8, 8]
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

