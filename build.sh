set -e

: "${CC:=tcc}"
: "${CFLAGS:=-O2}"

: "${GOCC:=$CC}"
: "${GOCFLAGS:=$CFLAGS}"

if [ "$USE_NOTCURSES" = "1" ]; then
    CFLAGS="$CFLAGS -DUSE_NOTCURSES"
    LDFLAGS="$LDFLAGS -lnotcurses -lnotcurses-core"
fi

if [ "$BUILD_LSP" = "1" ]; then
    CFLAGS="$CFLAGS -DBUILD_LSP"
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

    if [ "$BUILD_LSP" = "1" ]; then
        echo "# libsqanalysis.a"
        ./build.exe "libsqanalysis.a"

        OLDCC=$CC
        OLDCFLAGS=$CFLAGS
        echo "# sqlsp"
        CC=$GOCC CFLAGS=$GOCFLAGS ./build.exe "sqlsp"

        CC=$OLDCC
        CFLAGS=$OLDCFLAGS

        echo "# sq.exe"
        $CC $CFLAGS -DCC=\""$CC"\" -DCC_ARGS=\""$CFLAGS"\" -DLD_ARGS=\""$LDFLAGS sqlsp/sqlsp.a"\" build.c -o build.exe
        ./build.exe "sq.exe"
    else
        echo "# sq.exe"
        ./build.exe "sq.exe"
    fi
else
    ./build.exe "$1"
fi
