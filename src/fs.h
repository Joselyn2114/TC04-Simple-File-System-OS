//

#ifndef FS_H
#define FS_H

#include <stdbool.h>
#include <stddef.h>

#include "error.h"

#define MAX_FILES 100
#define MAX_FILENAME 32
#define BLOCK_SIZE 512
#define TOTAL_STORAGE 1048576  // 1024 * 1024, 1 MB
#define TOTAL_BLOCKS (TOTAL_STORAGE / BLOCK_SIZE)

typedef struct {
  bool in_use;
  char name[MAX_FILENAME];
  size_t size;
  size_t block_count;
  size_t* used_blocks;
} file_t;

typedef struct {
  file_t* files;
  char* data_blocks;
  bool* block_map;
} file_system_t;

err_t fs_init(file_system_t* fs);

err_t fs_create(file_system_t* fs, const char* name, size_t size);

err_t fs_delete(file_system_t* fs, const char* name);

void fs_free(file_system_t* fs);

#endif  // FS_H
