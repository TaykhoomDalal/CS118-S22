#!/bin/bash

# create the no loss output
./create_no_loss_out.sh

echo ""

# create the loss output
./create_loss_out.sh

# run the tests
./check_output.sh