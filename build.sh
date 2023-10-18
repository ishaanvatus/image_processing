#!/bin/bash

gcc -Wall -Wextra -ggdb improc.c main.c -O3 -o main -lm -mcmodel=medium
