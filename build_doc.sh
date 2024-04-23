set -e

doctimestamp=$(date +%s -r doc)
mkdir -p build/
if [ -f build/doc.time ]; then
    if [ $doctimestamp -eq $(cat build/doc.time) ]; then
        echo "skipping (build/doc.time timestamp match)"
        exit 0
    fi
fi
echo $doctimestamp > build/doc.time

CC="${VAR:-tcc}"

for d in doc/*/; do
    d=$(basename $d)
    mkdir -p build/doc/$d/
    for f in doc/$d/*.md; do 
        u-xxd/build/uxxd.exe -i $f -o build/$f.c
        $CC -c build/$f.c -o build/$f.o
    done
done

ar rcs build/doc.a build/doc/*/*.o

symbols=$(nm build/doc.a -f just-symbols | grep -v "_len")

echo "" > build/doc_lut.c

for sym in $symbols; do
    echo "extern unsigned char $sym[];" >> build/doc_lut.c
    echo "extern unsigned int ${sym}_len;" >> build/doc_lut.c
done

echo "void* doc_lut[][3] = {" >> build/doc_lut.c
for sym in $symbols; do
    name="${sym/_md/}"
    name="${name//_/ }"
    echo "  (void*[3]) { \"$name\", &$sym, &${sym}_len }," >> build/doc_lut.c
done
echo "  (void*[3]) { (void*)0, (void*)0, (void*)0 }" >> build/doc_lut.c
echo "};" >> build/doc_lut.c
