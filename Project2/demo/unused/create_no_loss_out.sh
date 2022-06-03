#!/bin/bash

TIMEFORMAT=%R

# start the server in the background
.././server 5000 > /dev/urandom &

echo "100B no loss"
time .././client localhost 5000 input/100B.txt > output/100B_no_loss_client.log
mv 1.file output/100B_no_loss_transfer.log

sleep 2

echo "100KB no loss"
time .././client localhost 5000 input/100KB.txt > output/100KB_no_loss_client.log
mv 2.file output/100KB_no_loss_transfer.log

sleep 2

echo "1MB no loss"
time .././client localhost 5000 input/1MB.bin > output/1MB_no_loss_client.log
mv 3.file output/1MB_no_loss_transfer.log

./kill_procs.sh