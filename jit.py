#! /usr/bin/env python

import os
import sys
import subprocess

CACHE_FOLDER = '__cint_cache__'
CACHE_CODE_PIECE_FOLDER = 'code_piece'
CACHE_SHARED_LIB_FOLDER = 'shared_lib'

DEFINE = '#define'
UNDEFINE = '#undef'
JIT_KEYWORD = '__JIT__'

code_full_path = sys.argv[1]
code_path, code_name = os.path.split(code_full_path)
cache_dir_path = os.path.join(code_path, CACHE_FOLDER)
piece_dir_path = os.path.join(cache_dir_path, CACHE_CODE_PIECE_FOLDER)
shared_lib_dir_path = os.path.join(cache_dir_path, CACHE_SHARED_LIB_FOLDER)

# check if the cache folders exist, if not, create them
if not os.path.exists(cache_dir_path):
    os.mkdir(cache_dir_path)
if not os.path.exists(piece_dir_path):
    os.mkdir(piece_dir_path)
if not os.path.exists(shared_lib_dir_path):
    os.mkdir(shared_lib_dir_path)

# check if create/update code pieces is necessary for JIT
code_piece_path = os.path.join(piece_dir_path,
                               os.path.splitext(code_name)[0] + '.c')
shared_lib_path = os.path.join(shared_lib_dir_path,
                               os.path.splitext(code_name)[0] + '.so')
need_rebuild = False
if not os.path.exists(code_piece_path)\
   or os.path.getmtime(code_piece_path) < os.path.getmtime(code_full_path):
   need_rebuild = True
if not os.path.exists(shared_lib_path)\
   or os.path.getmtime(shared_lib_path) < os.path.getmtime(code_full_path):
   need_rebuild = True

# build shared library if needed
if need_rebuild:

    # cut out the code pieces enclosed by "#define __JIT__"
    # and "#undef __JIT__"
    target_file = open(code_piece_path, 'w')
    source_file = open(code_full_path, 'r')
    lines = source_file.readlines()
    source_file.close()

    output_switch = False
    for line in lines:

        # turn on/off the switch if we are in/out of JIT scope
        if JIT_KEYWORD in line:
            if line.startswith(DEFINE):
                if output_switch:
                    raise RuntimeError('Starting JIT scope without ending the previous')
                output_switch = True
                continue
            if line.startswith(UNDEFINE):
                if not output_switch:
                    raise RuntimeError('Ending a JIT scope but never started')
                output_switch = False

        # write the code to the cache code file
        if output_switch:
            target_file.write(line)
    target_file.close()

    # compile the code piece to a shared library
    subp = subprocess.Popen(['gcc', '-O2', '-shared', '-fpic', '-o',
                             shared_lib_path, code_piece_path],
                             stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = subp.communicate()
    subp.wait()
