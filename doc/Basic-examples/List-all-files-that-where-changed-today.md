### List all files that where changed today
```shell
ls -l | sequencia -s 'split nl
ranges [1, -1]
map
  split " "
use []
  run "date"
  split " "
  select [1, 2]
axis
filter
  map 1
    select [5, 6]
  same
map
  select 1
  select 8
join nl'
```

