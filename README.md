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

For more advanced examples, take a look at the documentation.

## Documentation
- [Table of contents](doc/toc.md)

## Compiling
First download the repository via `git clone --recurse-submodules https://github.com/alex-s168/sequencia.git`.

If you already cloned it but forgot to specify the argument, run `git submodule update --init --recursive`.

The project uses [Build.C](https://github.com/alex-s168/build.c) as "build system".

To get started, make sure that you have:
- a functional C compiler
- `ar' tool

Run the `build.sh` script with optionally the `CC`, `CFLAGS` and `LDFLAGS` variables set (defaults to `tcc` and `-O2`) and optionally a task as argument (defaults to `all`).
The `build.sh` script automatically compiles the `build.c` script with the correct flags and then invokes it.

Tasks:
- `clean`
- `all` (default)
- `deps`
- `libsq.a`
- `sq.exe`
- `doc/text`
- `doc/glamour` (requires go)

The outputs will be in `build/`.

### Manually
Manually invoking each task gives you more control of your build process, like allowing for the use of glamour or using different compiler arguments for different tasks.
Steps that `all` does:
- `deps`
- `doc/text` (alternatively `doc/glamour`)
- `libsq.a`
- `sq.exe`

### Windows
I recommend installing git bash and tcc and then you can run all the linux steps in git bash without any problems.

