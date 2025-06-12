//

#include <stdio.h>

#include "fs.h"

int main() {
  file_system_t fs;

  fs_init(&fs);

  err_t result = OK;

  result = fs_create(&fs, "file1.txt", 128);
  if (result != OK) {
    return result;
  }

  result = fs_create(&fs, "file2.txt", 128);
  if (result != OK) {
    return result;
  }

  result = fs_delete(&fs, "file1.txt");
  if (result != OK) {
    return result;
  }

  result = fs_delete(&fs, "file3.txt");
  if (result != OK) {
    return result;
  }

  return result;
}
