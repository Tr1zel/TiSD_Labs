#!/bin/bash

gcc -std=c99 -Wall -Werror -Wpedantic -Wvla -Wextra -Wfloat-equal --coverage -c main.c
gcc -std=c99 -Wall -Werror -Wpedantic -Wvla -Wextra -Wfloat-equal --coverage -c IO.c
gcc -std=c99 -Wall -Werror -Wpedantic -Wvla -Wextra -Wfloat-equal --coverage -c funcs.c


gcc main.o IO.o funcs.o -o app.exe --coverage -lm
