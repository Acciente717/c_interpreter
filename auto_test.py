#! /usr/bin/env python

import os
import sys
import subprocess

CODE_EXTENSION = '.ic'
ANSWER_EXTENSION = '.ans'

all_code_file = os.listdir('test_cases')
all_ans_file = os.listdir('test_answers')

# check file extensions
for i in all_code_file:
    if os.path.splitext(i)[1] != CODE_EXTENSION:
        print('Unexpected file', i, 'in test_cases/')
        sys.exit(-1)
for i in all_ans_file:
    if os.path.splitext(i)[1] != ANSWER_EXTENSION:
        print('Unexpected file', i, 'in test_answers/')
        sys.exit(-1)

all_code_name = {os.path.splitext(i)[0] for i in all_code_file}
all_ans_name = {os.path.splitext(i)[0] for i in all_ans_file}

# check answers and test cases matching
if all_code_name != all_ans_name:
    print('Test cases and answers mismatch!')
    sys.exit(-1)

# run test cases
success_cnt = 0
test_cnt = 0
for name in all_code_name:
    test_cnt += 1

    # execute C code
    subp = subprocess.Popen('./cint',
                            stdin=open(os.path.join('test_cases', name+CODE_EXTENSION)),
                            stdout=subprocess.PIPE)
    stdout, stderr = subp.communicate()
    subp.wait()

    # move to program output and split it by lines
    stdout = str(stdout, encoding='utf8')
    idx = stdout.find('### start program execution ###\n')
    if idx == -1:
        print('Test cases', '`'+name+'`', 'has corrupted output!')
        continue
    idx += len('### start program execution ###\n')
    output_lines = stdout[idx:].split('\n')

    # open answer file
    answer_lines = []
    with open(os.path.join('test_answers', name+ANSWER_EXTENSION)) as ans_file:
        answer_lines = ans_file.readlines()
    answer_lines = [i.rstrip('\n') for i in answer_lines]

    # compare lines
    length = len(answer_lines)
    if len(output_lines) == 0:
        print('#%d ' % test_cnt, end='')
        print('Test case', '`'+name+'`', 'has empty program output!')
        continue
    if len(answer_lines) == 0:
        print('#%d ' % test_cnt, end='')
        print('Test case', '`'+name+'`', 'has empty answere file!')
        continue
    for i in range(length):
        if output_lines[i] != answer_lines[i]:
            print('#%d ' % test_cnt, end='')
            print('Test case', '`'+name+'`', 'has wrong output on line', i)
            print('Expect:', repr(answer_lines[i]))
            print('Get:   ', repr(output_lines[i]))
            print('')
            break
    else:
        success_cnt += 1
        print('#%d ' % test_cnt, end='')
        print('Test case', '`'+name+'`', 'succeeded!')

# output summary
print('All test cases are finished!')
print('Pass', '%d/%d' % (success_cnt, len(all_code_name)))
