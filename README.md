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

## Example
```
ls -l | sequencia -s 'split nl
map
  split " "
  ranges [5, 8]
  join " "
join nl'
```

For more advanced examples, take a look at [Basics](doc/how2basics.md).

## Documentation
- [Basics](doc/how2basics.md)
- [Operations](doc/operations.md)

## Compiling
First download the repository via `git clone --recurse-submodules https://github.com/alex-s168/sequencia.git`.

If you already cloned it but forgot to specify the argument, run `git submodule update --init --recursive`.

The project uses [Build.C](https://github.com/alex-s168/build.c) as "build system".

To get started, make sure that you have:
- a functional C compiler
- `ar' tool

Run the `build.sh` script with optionally the `CC` and `CFLAGS` variables set (defaults to `tcc` and `-O2`) and optionally a task as argument (defaults to `all`).
The `build.sh` script automatically compiles the `build.c` script with the correct flags and then invokes it.

Tasks:
- `clean`
- `all` (default)
- `deps`
- `libsq.a`
- `sq.exe`

The outputs will be in `build/`.

### Windows
I recommend installing git bash and tcc and then you can run all the linux steps in git bash without any problems.

