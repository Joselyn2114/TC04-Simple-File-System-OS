// src/main.c
//

#include <stdio.h>
#include <locale.h>      // ← para setlocale
#include "parser.h"

int main(int argc, char** argv) {
  // Inicializa localidad para UTF-8 y que 'ó', 'í', etc. se impriman bien
  setlocale(LC_ALL, "");

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
