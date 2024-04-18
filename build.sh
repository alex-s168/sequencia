set -e

: "${CC:=tcc}"
: "${CFLAGS:=-O2}"

echo "> kollektions.a and dependencies"
cd kollektions
$CC $CFLAGS -DCC=\""$CC"\" -DCFLAGS=\""$CFLAGS"\" build.c -o build.exe
./build.exe kallok.a
./build.exe kollektions.a
./build.exe kash.a
./build.exe tests || true
cd ..

echo "> libsq.a"
$CC $CFLAGS -DCC=\""$CC"\" -DCFLAGS=\""$CFLAGS"\" build.c -o build.exe

./build.exe libsq.a

echo "> sq.exe"
./build.exe sq.exe
