#include "output.h"

bool writebytes(unsigned long long x, int nbytes) {
  do {
    if (putchar(x) < 0)
      return false;
    x >>= CHAR_BIT;
    nbytes--;
  } while (0 < nbytes);

  return true;
}

int handle_output(char *input, char *output, int nbytes) {
  // Initialize local variables
  void (*initialize)(void);
  unsigned long long (*rand64)(void);
  void (*finalize)(void);

  // Error handling for missing input or output
  if (!output || !input) {
    fprintf(stderr, "Error: input/output");
    return 1;
  }

  // Handle input options
  if (strcmp(input, "rdrand") == 0) {
    // Handles hardware case
    if (rdrand_supported()) {
      initialize = hardware_rand64_init;
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
      initialize();
    } else {
      fprintf(stderr, "Error: rdrand");
      return 1;
    }
  } else if (strcmp(input, "mrand48_r") == 0) {
    // Handles mrand48_r case
    initialize = software_rand64_init;
    rand64 = software_rand64;
    finalize = software_rand64_fini;
    initialize();
  } else if (strncmp(input, "/", 1) == 0) {
    // Handles /F case
    file_software_rand64_init(input);
    rand64 = software_rand64;
    finalize = software_rand64_fini;
  } else {
    // Handle wrong input
    fprintf(stderr, "Error: invalid input");
    return 1;
  }

  int wordsize = sizeof rand64();
  int output_errno = 0;

  if (strcmp(output, "stdio") == 0) {
    // Default stdio option
    do {
      unsigned long long x = rand64();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes(x, outbytes)) {
        output_errno = errno;
        break;
      }
      nbytes -= outbytes;
    } while (0 < nbytes);
  } else {
    // Handle -o N option
    char *end_pointer;
        long block_size = strtol(output, &end_pointer, 10);
        if (*end_pointer != '\0' || block_size <= 0){
            fprintf(stderr, "Error: invalid output option '%s'\n", output);
            return 1;
        }
        unsigned char *buffer = malloc(block_size);
        if (!buffer){
            perror("malloc");
            return 1;
        }
        while (nbytes > 0){
            int current_block = (nbytes < block_size) ? nbytes : block_size;
            int i = 0;
            while (i < current_block){
                unsigned long long r = rand64();
                int copy_size = (current_block - i < wordsize) ? (current_block - i) : wordsize;
                memcpy(buffer + i, &r, copy_size);
                i += copy_size;
            }
            ssize_t written = write(STDOUT_FILENO, buffer, current_block);
            if (written < 0){
                output_errno = errno;
                break;
            }
            nbytes -= written;
        }
        free(buffer);
  }
  if (fclose(stdout) != 0)
    output_errno = errno;
  if (output_errno) {
    errno = output_errno;
    perror("output");
  }
  finalize();
  return output_errno;
}
