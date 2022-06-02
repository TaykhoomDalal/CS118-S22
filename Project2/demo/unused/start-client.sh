#!/bin/sh

# Usage: ./start-client.sh [PORT] [FILE_NAME]

./client 127.0.0.1 $1 $2 | tee -i client1.txt
# Or, use the following line instead when problem happens
# ./client 127.0.0.1 $1 $2 > client1.txt
