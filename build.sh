#!/bin/sh
set -xe

clang -I/usr/local/include -o gol gol.c -L/usr/local/lib -lraylib -lm 