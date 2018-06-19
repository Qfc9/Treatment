# make -C pretreatment
# make -C residential
make debug -C stormdrain
# make -C treatment
make debug -C dummy

cp stormdrain/stormdrain mounted/pretreatment/pretreatment
cp stormdrain/stormdrain mounted/stormdrain/stormdrain
cp stormdrain/stormdrain mounted/treatment/treatment
cp stormdrain/stormdrain mounted/residential/residential
cp dummy/dummy mounted/dummy