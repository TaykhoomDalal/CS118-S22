#!/bin/sh

echo 'Start of server output:'
head -n 20 server1.txt

echo
echo 'Start of client output:'
head -n 20 client1.txt

echo
echo -n 'Max SEND sequence number from client: '
awk 'BEGIN{max=0}{if ($1=="SEND" && $2>max) max=$2}END{print max}' client1.txt
