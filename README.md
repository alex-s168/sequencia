# Sequencia
Programming language for simple and easy data processing.

```
sequencia - Sequencia utility interpreter
  -S  --file [path]     REQUIRED: The path to a sequence script source file
  -s  --script [scrisequencia - Sequencia utility interpreter
  -S  --file [path]     REQUIRED: The path to a sequence script source file
  -s  --script [script] ^ ALTERNATIVE: The script to evaluate
  -I  --input [path]    The input file for data; default: "-"
  -d  --debug           Enable script developer debug outputpt] ^ ALTERNATIVE: The script to evaluate
  -I  --input [path]    The input file for data; default: "-"
  -d  --debug           Enable script developer debug output
```

## Examples
```
ls -l | sequencia -s 'split nl
map
  split " "
  select [5, 6, 7, 8]
  join " "
join nl'
```

For more advanced examples, take a look at [Basics](doc/how2basics.md).

## Documentation
- [Basics](doc/how2basics.md)
- [Operations](doc/operations.md)
