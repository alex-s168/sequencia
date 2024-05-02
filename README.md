# Sequencia
Programming language for simple and easy data processing.

```
sq - Sequencia utility interpreter
  -S  --file [path]     REQUIRED: The path to a sequence script source file
  -s  --script [script] ^ ALTERNATIVE: The script to evaluate
  -I  --input [path]    The input file for data; default: "-"
  -d  --debug-output    Enable debug output (ignored with interactive debugger)
  -g  --debugger        Start interactive debugger; Requires "-I"!
  -h  --help            Show this help message
      --doc (topic)     Print out the documentation for the given topic OR list all topics available
      --stats           Print memory allocation statistics
      --analyze         Run code analysis (requires script or script file to be specified)
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
- `ar` tool

Run the `build.sh` script with the following variables set:
- `CC` - C compiler - defaults to `tcc`
- `CFLAGS` - C compiler arguments - defaults to `-O2`
- `LDFLAGS` - C compiler arguments for linking - defaults to ``
- `USE_GLAMOUR` - 0 (false) or 1 (true) - defaults to `0` - requires go to be installed - Embedd glamour for rendering markdown for built-in doc
- `USE_NOTCURSES` - 0 (false) or 1 (true) - defaults to `0` - requires notcurses to be installed - Use notcurses to show a better list of doc topics

Example:
`CC=clang USE_GLAMOUR=1 ./build.sh`

Always run `./build.sh clean` after you change compiler or compiler arguments!

The outputs will be in `build/`.

### Windows
I recommend installing git bash and tcc and then you can run all the linux steps in git bash without any problems.

