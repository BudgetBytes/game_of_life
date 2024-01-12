#!/bin/sh
set -xe

cc -I/usr/local/include -Wall -Wextra -g -o game_of_life game_of_life.c -L/usr/local/lib -lraylib -lm 