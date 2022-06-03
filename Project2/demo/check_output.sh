#!/bin/bash

# Test 1:
echo 'Test 1: 100B no loss client'
python3 tests/test_no_loss.py output/100B_no_loss_client.log

# Test 2:
echo 'Test 2: 100KB no loss client'
python3 tests/test_no_loss.py output/100KB_no_loss_client.log

# Test 3:
echo 'Test 3: 1MB no loss client'
python3 tests/test_no_loss.py output/1MB_no_loss_client.log

# Test 4:
echo 'Test 4: 100B loss client'
python3 tests/test_loss_gbn.py output/100B_loss_client.log

# Test 5:
echo 'Test 5: 100KB loss client'
python3 tests/test_loss_gbn.py output/100KB_loss_client.log

# Test 6:
echo 'Test 6: 1MB loss client'
python3 tests/test_loss_gbn.py output/1MB_loss_client.log