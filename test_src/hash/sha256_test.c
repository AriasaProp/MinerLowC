#include "common.h"
#include "hash.h"

#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

extern int apply;

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
  sha256 tsha256;
  while (!feof(file_pointer)) {
    fgets(line, sizeof(line), file_pointer);
    endline = strchr(line, '\n');
    if (endline) *endline = 0;
    hash_sha256(&tsha256, line, strlen(line));
    printf("%01d->%s (%lu) (%d)\n", i, line, strlen(line), apply);
    fgets(line, sizeof(line), file_pointer);
    endline = strchr(line, '\n');
    if (endline) *endline = 0;
    printf("h->%s\n", line);
    bytesToHex(line, tsha256.bytes, 256);
    printf("i->%s\n", line);
    ++i;
  }
  fclose(file_pointer);
  printf("done!\n");
  return 0;
}
