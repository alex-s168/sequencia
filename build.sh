set -e

: "${CC:=tcc}"
: "${CFLAGS:=-O2}"

if [ "$USE_NOTCURSES" = "1" ]; then
    CFLAGS="$CFLAGS -DUSE_NOTCURSES"
    LDFLAGS="$LDFLAGS -lnotcurses -lnotcurses-core"
fi

$CC $CFLAGS -DCC=\""$CC"\" -DCC_ARGS=\""$CFLAGS"\" -DLD_ARGS=\""$LDFLAGS"\" build.c -o build.exe

if [ -z "$1" ]; then
    echo "# deps"
    ./build.exe "deps"

    if [ "$USE_GLAMOUR" = "1" ]; then
        echo "# doc/glamour"
        ./build.exe "doc/glamour"
    else
        echo "# doc/text"
        ./build.exe "doc/text"
    fi

    echo "# libsq.a"
    ./build.exe "libsq.a"

    echo "# libsqanalysis.a"
    ./build.exe "libsqanalysis.a"

    echo "# sq.exe"
    ./build.exe "sq.exe"
else
    ./build.exe "$1"
fi
