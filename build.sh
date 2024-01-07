#!/bin/sh
set -xe

clang -I/usr/local/include -o main main.c -L/usr/local/lib -lraylib -lm 