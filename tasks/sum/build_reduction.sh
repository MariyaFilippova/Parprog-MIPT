#!/bin/bash
gcc -Xpreprocessor -fopenmp -lomp sum_reduction.c -o sum_reduction