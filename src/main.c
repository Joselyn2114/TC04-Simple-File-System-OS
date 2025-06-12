//

#include <stdio.h>

#include "fs.h"

int main() {
  err_t error = OK;

  file_system_t fs;

  if (!error) {
    error = fs_init(&fs);
  }

  if (!error) {
    error = fs_create(&fs, "file1.txt", 1024);

    if (!error) {
      printf("File 'file1.txt' created successfully.\n");
    }
  }

  if (!error) {
    error = fs_create(&fs, "file2.txt", 2048);

    if (!error) {
      printf("File 'file2.txt' created successfully.\n");
    }
  }

  if (!error) {
    error = fs_delete(&fs, "file1.txt");

    if (!error) {
      printf("File 'file1.txt' deleted successfully.\n");
    }
  }

  if (!error) {
    error = fs_delete(&fs, "file3.txt");

    if (!error) {
      printf("File 'file3.txt' deleted successfully.\n");
    }
  }

  fs_free(&fs);

  return error;
}
