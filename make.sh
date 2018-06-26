# make -C pretreatment
# make -C residential
make debug -C stormdrain
# make -C treatment
make debug -C dummy

cp stormdrain/stormdrain mount/stormdrain/stormdrain
cp stormdrain/stormdrain mount/treatment/treatment
cp stormdrain/stormdrain mount/residential/residential

cp lib/* mount/pretreatment/lib
cp pretreatment/* mount/pretreatment/source

rm mount/pretreatment/lib/*.o
rm mount/pretreatment/source/*.o

# cp dummy/dummy remote/