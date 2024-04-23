### Parse `ls -l` output
```shell
ls -l | sequencia -s 'split nl
ranges [1, -1]
map
  split " "
  select 8
join nl'
```
We need the `ranges [1, -1]` at the beginning,
to get rid of the total amount of files.

