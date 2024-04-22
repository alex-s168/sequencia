### List all C files in the current directory
```shell
ls -l | sequencia -s 'split nl
ranges [1, -1]
map
  split " "
  select 8
filter
  contains ".c"
join nl'
```

