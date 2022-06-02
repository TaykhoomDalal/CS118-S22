#!/usr/bin/env python3

"""
Usage: ./test_loss_gbn.py output_file or python3 test_loss_gbn.py output_file
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

def check_timeout(all_lines) -> bool:
    is_timeout = False
    resend_counter = 0
    step = 512
    timeout_seq = 0
    is_syn_timeout = False
    is_syn_third_pkt_timeout = False
    is_fin_timeout = False
    no_timeout = True

    my_isn = None
    my_fin = None

    pass_check = True
    total_num = len(all_lines)

    for lino, line in enumerate(all_lines):
        if len(line) == 0:
            continue
        result = output_pattern.match(line)
        result = result.groupdict()

        # SEND SYN, get ISN
        if result['event'] == 'SEND' and result['syn_flag'] is not None:
            my_isn = int(result['seq'])

        # SEND FIN
        if result['event'] == 'SEND' and result['fin_flag'] is not None:
            my_fin = int(result['seq'])

        # Record TIMEOUT
        if result['event'] == 'TIMEOUT':
            no_timeout = False
            if int(result['seq']) == my_isn:
                is_syn_timeout = True
            elif int(result['seq']) == (my_isn + 1) % 25601:
                is_syn_third_pkt_timeout = True
            elif int(result['seq']) == my_fin:
                is_fin_timeout = True
            else:
                is_timeout = True
                timeout_seq = int(result['seq'])
                timeout_lino = lino
                resend_counter = 0
            continue

        if is_timeout and resend_counter < 10:
            if result['event'] == 'RECV':
                continue
            elif result['event'] == 'RESEND' and int(result['seq']) != (timeout_seq + step * resend_counter) % 25601:
                # print('bad resend seq: %d, %d'%(int(result['seq']), (timeout_seq + step * resend_counter) % 25601))
                print('[Error] Wrong seq in resent data: line', lino+1, line)
                pass_check = False
                # return False
            elif result['event'] == 'RESEND' and int(result['seq']) == (timeout_seq + step * resend_counter) % 25601:
                resend_counter += 1
                if resend_counter is 10:
                    is_timeout = False
                    resend_counter = 0
            elif result['event'] == 'SEND':
                seq = int(result['seq'])
                if seq < (timeout_seq + step * resend_counter) % 25601:
                    is_timeout = False
                else:
                    print('[Warning] suspicious resend_counter', resend_counter)

    if is_timeout:
        print('[Warning] DATA timeout not solved at line #', timeout_lino+1)
        pass_check = False
        # return False

    if no_timeout:
        print('[Warning] timeout not triggered.')
        pass_check = False
    return pass_check


if __name__ == "__main__":
    with open(get_filename()) as f:
        all_lines = f.readlines()

    check_result = check_timeout(all_lines)
    print("Check result: ", check_result)
