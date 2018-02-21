#!/usr/bin/env python3

import os, sys
from itertools import chain
from glob import iglob

sv_exports = []
cl_exports = []

mod_dir = sys.argv[1]
sv_dir = mod_dir + "/dlls"
cl_dir = mod_dir + "/cl_dll"

def dedup(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if not (x in seen or seen_add(x))]

def get_exports(explist, f, fname):
    lnum = 1
    for line in f:
        line = line.strip()
        exp = ''
        if line.startswith('LINK_ENTITY_TO_CLASS'):
            exp = line[len('LINK_ENTITY_TO_CLASS'):].strip()
        elif 'LINK_ENTITY_TO_CLASS' in line:
            print(fname + ':' + str(lnum) + ': bogus export? ', line)
            continue
        else:
            continue
        expl = exp[1:].split(',')
        if len(expl) == 0:
            print(fname + ':' + str(lnum) + ': bogus export? ', line)
            continue
        exp = expl[0].strip()
        explist.append(exp)
        lnum += 1

sv_files = chain(
    iglob(sv_dir + '/**/*.cpp', recursive=True),
    iglob(sv_dir + '/**/*.c', recursive=True),
    iglob(sv_dir + '/**/*.h', recursive=True)
)
cl_files = chain(
    iglob(cl_dir + '/**/*.cpp', recursive=True),
    iglob(cl_dir + '/**/*.c', recursive=True),
    iglob(cl_dir + '/**/*.h', recursive=True)
)

for fname in sv_files:
    with open(fname) as f:
        get_exports(sv_exports, f, fname)
dedup(sv_exports)

for exp in sv_exports:
    print('void ' + exp + '( entvars_t *pev );')

for exp in sv_exports:
    print('{ "' + exp + '", (void*)' + exp + ' },')
