# make -C pretreatment
# make -C residential
make debug -C stormdrain
# make -C treatment
make debug -C dummy

cp stormdrain/stormdrain mounted/stormdrain/stormdrain
cp stormdrain/stormdrain mounted/treatment/treatment
cp stormdrain/stormdrain mounted/residential/residential

cp lib/* mounted/pretreatment/lib
cp pretreatment/* mounted/pretreatment/source

rm mounted/pretreatment/lib/*.o
rm mounted/pretreatment/source/*.o

# cp dummy/dummy remote/