# List TODOs added / removed in a git repo
```shell
echo "$@" | sq -s '
make 2
map 1
  prepend "git --no-pager diff --name-only "
  with
    run
  split nl
flatten
axis
map
  map 0
    prepend "git diff "
  join " "
  with
    run
  split nl
  make 2
  map 0
    select 0
  map 1
    ranges [4, -1]
    join nl
    run "grep TODO"
    split nl
    make 2
    map 1
      len
      invert
    map 0
      join nl
  flatten
  ranges [-1, -1, 0, -2]
  map 0
    make 1
    append -1
  with
    ranges
  map
    noempty
  join nl
  noempty
map
  append nl
join nl
' 2>/dev/null
```
