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

eharmon@tdqc4:~/submit/treatmentPlant/stormdrain$ scp -P 3066 stormdrain root@10.15.30.4:/root/resident
root@10.15.30.4's password: 
stormdrain                                               100%  103KB 102.6KB/s   00:00    
eharmon@tdqc4:~/submit/treatmentPlant/stormdrain$ scp -P 3130 stormdrain root@10.15.30.4:/root/stormdrain
root@10.15.30.4's password: 
stormdrain                                               100%  103KB 102.6KB/s   00:00    
eharmon@tdqc4:~/submit/treatmentPlant/stormdrain$ scp -P 3001 stormdrain root@10.15.30.4:/root/treatment
root@10.15.30.4's password: 
stormdrain                                               100%  103KB 102.6KB/s   00:00    
eharmon@tdqc4:~/submit/treatmentPlant/stormdrain$ 
