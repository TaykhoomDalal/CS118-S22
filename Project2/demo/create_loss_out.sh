#!/bin/bash

TIMEFORMAT=%R

# start the server in the background and the proxy script
.././server 5000 > /dev/urandom &
python3 ../rdproxy.py 5000 9999 0.1 &

echo "100B loss"
time .././client localhost 9999 input/100B.txt > output/100B_loss_client.log
mv 1.file output/100B_loss_transfer.log

echo "100KB loss"
time .././client localhost 9999 input/100KB.txt > output/100KB_loss_client.log
mv 2.file output/100KB_loss_transfer.log

echo "1MB loss"
time .././client localhost 9999 input/1MB.bin > output/1MB_loss_client.log
mv 3.file output/1MB_loss_transfer.log

./kill_procs.sh