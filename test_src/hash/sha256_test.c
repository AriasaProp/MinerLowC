#include "common.h"
#include "hash.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

int main (int argc, const char **argv) {
  if (argc != 2) {
    perror("invalid arguments\n");
    return 1;
  }
  char line[512]; 
  FILE *file_pointer = fopen(argv[1], "r");
  
  if (!file_pointer) {
    perror("cannot open file.\n");
    return 1;
  }
  
  int i = 0;
  char *endline;
  sha256 A, B;
  while (!feof(file_pointer)) {
    fgets(line, sizeof(line), file_pointer);
    endline = strchr(line, '\n');
    if (endline) *endline = 0;
    hash_sha256(A.bytes, line, strlen(line));
    printf("%01d->%s (%lu)\n", i, line, strlen(line));
    fgets(line, sizeof(line), file_pointer);
    endline = strchr(line, '\n');
    if (endline) *endline = 0;
    hexToBytes(B.bytes, line, 256);
    if (memcmp(B.bytes, A.bytes, 32)) {
      bytesToHex(line, B.bytes, 256);
      printf(">%s\n", line);
      bytesToHex(line, A.bytes, 256);
      printf(">%s\n", line);
    } else {
      printf(">Equals!!!\n");
    }
    ++i;
  }
  fclose(file_pointer);
  printf("done!\n");
  return 0;
}
