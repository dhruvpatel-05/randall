#ifndef RAND64_SW_H
#define RAND64_SW_H
#include <stdio.h>
#include <stdlib.h>

void software_rand64_init(void);

void file_software_rand64_init(char *filename);

unsigned long long software_rand64(void);

void software_rand64_fini(void);

#endif