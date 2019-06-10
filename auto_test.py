#! /usr/bin/env python

import os
import sys
import subprocess

INTERPRETED_CODE_EXTENSION = '.ic'
GROUNDTRUTH_CODE_EXTENSIOON = '.c'
GROUHDTRUTH_RUNNABLE_FILE_EXTENSION = '.truth'
GROUNDTRHTH_EXCLUDE_EXTENSION = '.h'

TEST_CASE_DIR = 'test_cases/'
GROUNDTRUTH_DIR = 'test_groundtruth/'

all_code_file = os.listdir(TEST_CASE_DIR)
all_groundtruth_file = os.listdir(GROUNDTRUTH_DIR)

# check file extensions
for i in all_code_file:
    if os.path.splitext(i)[1] != INTERPRETED_CODE_EXTENSION:
        print('\033[31m', end='') # red start
        print('Unexpected file', i, 'in ' + TEST_CASE_DIR)
        print('\033[0m', end='') # red end
        sys.exit(-1)
for i in all_groundtruth_file:
    if os.path.splitext(i)[1] != GROUNDTRUTH_CODE_EXTENSIOON\
    and os.path.splitext(i)[1] != GROUHDTRUTH_RUNNABLE_FILE_EXTENSION\
    and os.path.splitext(i)[1] != GROUNDTRHTH_EXCLUDE_EXTENSION:
        print('\033[31m', end='') # red start
        print('Unexpected file', i, 'in ' + GROUNDTRUTH_DIR)
        print('\033[0m', end='') # red end
        sys.exit(-1)

all_test_code_name = {os.path.splitext(i)[0] for i in all_code_file}
all_groudtruth_code_name = {os.path.splitext(i)[0]
                            for i in all_groundtruth_file
                            if os.path.splitext(i)[1] == GROUNDTRUTH_CODE_EXTENSIOON}

# check answers and test cases matching
if all_test_code_name != all_groudtruth_code_name:
    print('\033[31m', end='') # red start
    print('Test cases and groundtruth mismatch!')
    print('\033[0m', end='') # red end
    sys.exit(-1)

# generate goundtruth executable files
print('Preparing groundtruth executables...')
for i in all_groudtruth_code_name:
    if i + GROUHDTRUTH_RUNNABLE_FILE_EXTENSION in all_groundtruth_file:
        executable_path = os.path.join(GROUNDTRUTH_DIR, i + GROUHDTRUTH_RUNNABLE_FILE_EXTENSION)
        executable_mtime = os.path.getmtime(executable_path)
        code_path = os.path.join(GROUNDTRUTH_DIR, i + GROUNDTRUTH_CODE_EXTENSIOON)
        code_mtime = os.path.getmtime(code_path)

        if code_mtime < executable_mtime:
            print('\033[32m', end='') # green start
            print(executable_path, 'is up-to-date')
            print('\033[0m', end='') # green end
            continue

    print('\033[33m', end='') # yello start
    print('Recompiling', os.path.join(GROUNDTRUTH_DIR, i + GROUHDTRUTH_RUNNABLE_FILE_EXTENSION))
    print('\033[0m', end='') # yello end
    subp = subprocess.Popen(['gcc', '-O2', '-o',
                             os.path.join(GROUNDTRUTH_DIR, i+GROUHDTRUTH_RUNNABLE_FILE_EXTENSION),
                             os.path.join(GROUNDTRUTH_DIR, i+GROUNDTRUTH_CODE_EXTENSIOON)],
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    subp.communicate()
    subp.wait()
    if subp.returncode != 0:
        print('\033[31m', end='') # red start
        print('Error occured while compiling',
              os.path.join(GROUNDTRUTH_DIR, i+GROUNDTRUTH_CODE_EXTENSIOON))
        print('\033[0m', end='') # red end
        sys.exit(-1)

# run test cases
print('')
print('Starting test...')
success_cnt = 0
test_cnt = 0
for name in all_groudtruth_code_name:
    test_cnt += 1

    # interpret C code
    subp = subprocess.Popen('./cint',
                            stdin=open(os.path.join('test_cases', name+INTERPRETED_CODE_EXTENSION)),
                            stdout=subprocess.PIPE)
    cint_stdout, cint_stderr = subp.communicate()
    subp.wait()

    # move to program output and split it by lines
    cint_stdout = str(cint_stdout, encoding='utf8')
    idx = cint_stdout.find('### start program execution ###\n')
    if idx == -1:
        print('\033[31m', end='') # red start
        print('Test cases', '`'+name+'`', 'has corrupted output!')
        print('\033[0m', end='') # red end
        continue
    idx += len('### start program execution ###\n')
    output_lines = cint_stdout[idx:].split('\n')

    # execute groundtruth program
    subp = subprocess.Popen(os.path.join(GROUNDTRUTH_DIR, name+GROUHDTRUTH_RUNNABLE_FILE_EXTENSION),
                            stdout=subprocess.PIPE)
    groundtruth_stdout, groundtruth_stderr = subp.communicate()
    subp.wait()
    groundtruth_stdout = str(groundtruth_stdout, encoding='utf8')
    answer_lines = groundtruth_stdout.split('\n')

    # compare lines
    length = len(answer_lines)
    if len(output_lines) == 0:
        print('\033[31m', end='') # red start
        print('#%d ' % test_cnt, end='')
        print('Test case', '`'+name+'`', 'has empty program output!')
        print('\033[0m', end='') # red end
        continue
    if len(answer_lines) == 0:
        print('\033[31m', end='') # red start
        print('#%d ' % test_cnt, end='')
        print('Test case', '`'+name+'`', 'has empty answere file!')
        print('\033[0m', end='') # red end
        continue
    for i in range(length):
        if output_lines[i] != answer_lines[i]:
            print('\033[31m', end='') # red start
            print('#%d ' % test_cnt, end='')
            print('Test case', '`'+name+'`', 'has wrong output on line', i)
            print('Expect:', repr(answer_lines[i]))
            print('Get:   ', repr(output_lines[i]))
            print('')
            print('\033[0m', end='') # red end
            break
    else:
        success_cnt += 1
        print('\033[32m', end='') # green start
        print('#%d ' % test_cnt, end='')
        print('Test case', '`'+name+'`', 'succeeded!')
        print('\033[0m', end='') # green end

# output summary
print('')
print('All test cases are finished!')
print('Pass', '%d/%d' % (success_cnt, len(all_groudtruth_code_name)))
