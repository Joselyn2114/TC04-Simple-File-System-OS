//

#include <stdio.h>

#include "parser.h"

int main(int argc, char** argv) {
  err_t error = OK;

  if (argc < 2) {
    fprintf(stderr, "Usage: %s <file>.\n", argv[0]);
    return ERR_CMD_NO_ARGUMENT;
  }

  char* filename = argv[1];
  FILE* file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error: Can't open file '%s'.\n", filename);
    return ERR_FILE_NOT_FOUND;
  }

  file_system_t fs;

  error = fs_init(&fs);

  if (!error) {
    error = parser_start(&fs, file);
  }

  fclose(file);

  fs_free(&fs);

  return error;
}
