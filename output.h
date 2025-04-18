#ifndef OUTPUT_H
#define OUTPUT_H

#include "rand64-hw.h"
#include "rand64-sw.h"
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool writebytes(unsigned long long x, int nbytes);
int handle_output(char *input, char *output, int nbytes);
#endif