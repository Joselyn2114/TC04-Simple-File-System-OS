//

#include "fs.h"

#include <stdio.h>
#include <string.h>

#define NOT_FOUND_INDEX ((size_t) -1)

size_t find_file(const file_system_t* fs, const char* name) {
  for (size_t i = 0; i < MAX_FILES; i++) {
    if (fs->files[i].in_use && strcmp(fs->files[i].name, name) == 0) {
      return i;
    }
  }

  return NOT_FOUND_INDEX;
}

void fs_init(file_system_t* fs) {
  for (size_t i = 0; i < MAX_FILES; i++) {
    fs->files[i].in_use = false;
  }

  for (size_t i = 0; i < TOTAL_BLOCKS; i++) {
    fs->block_map[i] = false;
  }
}

err_t fs_create(file_system_t* fs, const char* name, size_t size) {
  if (find_file(fs, name) != NOT_FOUND_INDEX) {
    fprintf(stderr, "Error: file '%s' already exists.\n", name);
    return ERR_FILE_ALREADY_EXISTS;
  }

  size_t file_index = NOT_FOUND_INDEX;
  for (size_t i = 0; i < MAX_FILES; i++) {
    if (!fs->files[i].in_use) {
      file_index = i;
      break;
    }
  }

  if (file_index == NOT_FOUND_INDEX) {
    fprintf(stderr, "Error: Maximum number of files reached.\n");
    return ERR_MAX_FILES_REACHED;
  }

  size_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

  file_t* file = &fs->files[file_index];
  file->block_count = 0;

  size_t blocks_used = 0;
  for (size_t i = 0; i < TOTAL_BLOCKS && blocks_used < blocks_needed; i++) {
    if (!fs->block_map[i]) {
      fs->block_map[i] = true;
      file->used_blocks[blocks_used] = i;
      blocks_used++;
    }
  }

  if (blocks_used < blocks_needed) {
    for (size_t i = 0; i < blocks_used; i++) {
      fs->block_map[file->used_blocks[i]] = false;
    }

    fprintf(stderr, "Error: Not enough space to create file '%s'.\n", name);
    return ERR_NOT_ENOUGH_SPACE;
  }

  file->in_use = true;
  strncpy(file->name, name, MAX_FILENAME - 1);
  file->name[MAX_FILENAME - 1] = '\0';
  file->size = size;
  file->block_count = blocks_needed;

  return OK;
}

err_t fs_delete(file_system_t* fs, const char* name) {
  size_t file_index = find_file(fs, name);

  if (file_index == NOT_FOUND_INDEX) {
    fprintf(stderr, "Error: File '%s' not found.\n", name);
    return ERR_FILE_NOT_FOUND;
  }

  file_t* file = &fs->files[file_index];

  for (size_t i = 0; i < file->block_count; i++) {
    fs->block_map[file->used_blocks[i]] = false;
  }

  file->in_use = false;

  return OK;
}
