set -e

: "${CC:=tcc}"
: "${CFLAGS:=-O2}"
task=${1:-"all"}

if [ "$USE_NOTCURSES" = "1" ]; then
    CFLAGS="$CFLAGS -DUSE_NOTCURSES"
    LDFLAGS="$LDFLAGS -lnotcurses -lnotcurses-core"
fi

$CC $CFLAGS -DCC=\""$CC"\" -DCC_ARGS=\""$CFLAGS"\" -DLD_ARGS=\""$LDFLAGS"\" build.c -o build.exe
./build.exe $task
