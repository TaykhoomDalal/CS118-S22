#!/usr/bin/env python3

"""
Usage: ./test_no_loss.py output_file or python3 test_no_loss.py output_file
"""

import sys
import re

all_lines = None
output_pattern = re.compile(
    r'(?P<event>\w+) (?P<seq>\d+)\s?(?P<ack>\d+)?\s?(?P<syn_flag>SYN)?\s?(?P<fin_flag>FIN)?\s?(?P<ack_flag>ACK)?\s?(?P<dup_ack_flag>DUP-ACK)?')

def get_filename() -> str:
    if len(sys.argv) < 2:
        print(
            "Usage:", sys.argv[0], "output_file_name \n", file=sys.stderr)
        exit(-1)
    return sys.argv[1]

def check_format(all_lines) -> bool:

    pass_check = True
    total_num = len(all_lines)
    print('[Info] Total log line #', total_num)
    print('[Info] Estimated file size', (total_num / 2 -3) * 512)
    send_sn_lst = []
    recv_ack_lst = []
    for lino, line in enumerate(all_lines):
        if len(line) == 0 or lino > total_num - 25:
            continue
        result = output_pattern.match(line)
        result = result.groupdict()

        # SEND DATA
        if result['event'] == 'SEND':
            send_sn_lst.append(int(result['seq']))

        # RECV ACK
        elif result['event'] == 'RECV' and result['ack_flag']:
            if send_sn_lst and recv_ack_lst and send_sn_lst[-1] != (recv_ack_lst[-1] + 512*9) % 25601:
                print('[Error] Mismatch window size', recv_ack_lst[-1], send_sn_lst[-1], 'at line #', lino)
                pass_check = False
            recv_ack_lst.append(int(result['ack']))
            if int(result['ack']) >= 25601:
                print('[Error] Exceed max seq num', recv_ack_lst[-1], 'at line #', lino)
                pass_check = False

    # return True
    return pass_check


if __name__ == "__main__":
    with open(get_filename()) as f:
        all_lines = f.readlines()

    check_result = check_format(all_lines)
    print("Check result: ", check_result)
