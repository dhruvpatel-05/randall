
#include "rand64-sw.h"

/* Software implementation.  */

/* Input stream containing random bytes.  */
FILE *urandstream;

/* Initialize the software rand64 implemm,entation.  */
void software_rand64_init(void) {
  urandstream = fopen("/dev/random", "r");
  if (!urandstream)
    abort();
}

void file_software_rand64_init(char *filename){
  urandstream = fopen(filename, "r");
  if (!urandstream)
    abort();
}

/* Return a random value, using software operations.  */
unsigned long long software_rand64(void) {
  unsigned long long int x;
  if (fread(&x, sizeof x, 1, urandstream) != 1)
    abort();
  return x;
}

/* Finalize the software rand64 implementation.  */
void software_rand64_fini(void) { fclose(urandstream); }
