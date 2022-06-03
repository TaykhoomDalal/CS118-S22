#!/bin/bash

# Test 1:
printf 'Test 1: 100B no loss client\n'
python3 tests/test_no_loss.py output/100B_no_loss_client.log
diff input/100B.txt output/100B_no_loss_transfer.log

# Test 2:
printf '\nTest 2: 100KB no loss client\n'
python3 tests/test_no_loss.py output/100KB_no_loss_client.log
diff input/100KB.txt output/100KB_no_loss_transfer.log

# Test 3:
printf '\nTest 3: 1MB no loss client\n'
python3 tests/test_no_loss.py output/1MB_no_loss_client.log
diff input/1MB.bin output/1MB_no_loss_transfer.log

# Test 4:
printf '\nTest 4: 100B loss client\n'
python3 tests/test_loss_gbn.py output/100B_loss_client.log
diff input/100B.txt output/100B_loss_transfer.log

# Test 5:
printf '\nTest 5: 100KB loss client\n'
python3 tests/test_loss_gbn.py output/100KB_loss_client.log
diff input/100KB.txt output/100KB_loss_transfer.log

# Test 6:
printf '\nTest 6: 1MB loss client\n'
python3 tests/test_loss_gbn.py output/1MB_loss_client.log
diff input/1MB.bin output/1MB_loss_transfer.log