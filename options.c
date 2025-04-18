#include "options.h"

long long handle_nbytes(int argc, char **argv) {
  if (optind >= argc) {
    fprintf(stderr, "Error: NBYTES arg is missing");
    exit(EXIT_FAILURE);
  }
  bool valid = false;
  long long nbytes;
  char *endptr;
  errno = 0;
  nbytes = strtoll(argv[optind], &endptr, 10);
  if (errno) {
    perror(argv[optind]);
  } else {
    valid = !*endptr && 0 <= nbytes;
  }
  if (!valid) {
    fprintf(stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
    exit(EXIT_FAILURE);
  }
  return nbytes;
}