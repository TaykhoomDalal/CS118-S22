#!/bin/bash

pkill -f ".././server 5000"
pkill -f "python3 ../rdproxy.py 5000 9999 0.1"
