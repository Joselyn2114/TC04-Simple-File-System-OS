//

#ifndef FS_H
#define FS_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_FILES 100
#define MAX_FILENAME 32
#define BLOCK_SIZE 512
#define TOTAL_STORAGE 1048576  // 1024 * 1024, 1 MB
#define TOTAL_BLOCKS (TOTAL_STORAGE / BLOCK_SIZE)

typedef enum {
  OK = 0,
  ERR_FILE_ALREADY_EXISTS,
  ERR_MAX_FILES_REACHED,
  ERR_NOT_ENOUGHT_SPACE,
  ERR_FILE_NOT_FOUND,
} err_t;

typedef struct {
  bool in_use;
  char name[MAX_FILENAME];
  size_t size;
  size_t block_count;
  size_t used_blocks[TOTAL_BLOCKS];
} file_t;

typedef struct {
  file_t files[MAX_FILES];
  char data_blocks[TOTAL_BLOCKS][BLOCK_SIZE];
  bool block_map[TOTAL_BLOCKS];
} file_system_t;

void fs_init(file_system_t* fs);

err_t fs_create(const char* name, size_t size);

err_t fs_delete(const char* name);

#endif  // FS_H
