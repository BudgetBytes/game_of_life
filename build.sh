#!/bin/sh
set -xe

clang -I/usr/local/include -Wall -Wextra -o game_of_life game_of_life.c -L/usr/local/lib -lraylib -lm 