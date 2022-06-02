#!/bin/bash

if [ "$#" -eq 0 ]
then
  echo "Usage: ./set-loss.sh LossRate (optional)InferfaceName=lo"
  echo "Example:"
  echo "  ./set-loss.sh 10 : set 10% loss"
  echo "  ./set-loss.sh 20 lo0 : set 20% loss to lo0 interface"
  exit 1
fi

LOSSRATE=$1

if [ "$#" -eq 2 ]
then
  LOCALIF=$2
else
  LOCALIF="lo"
fi

if [[ $LOSSRATE == 0 ]]
then
  echo Reset loss rate to be $LOSSRATE\%
  sudo tc qdisc delete dev $LOCALIF root
else
  echo Set loss rate to be $LOSSRATE\%
  sudo tc qdisc add dev $LOCALIF root netem loss $LOSSRATE\%
fi

sudo tc qdisc show dev lo