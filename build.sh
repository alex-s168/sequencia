set -e

: "${CC:=tcc}"
: "${CFLAGS:=-O2}"
task=${1:-"all"}

$CC $CFLAGS -DCC=\""$CC"\" -DCFLAGS=\""$CFLAGS"\" build.c -o build.exe
./build.exe $task
