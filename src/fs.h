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


// Escribe 'data_len' bytes de 'data' en el archivo 'name' a partir de 'offset'
// - Sólo sobrescribe bytes dentro del tamaño declarado
err_t fs_write(file_system_t* fs,
               const char* filename,
               size_t offset,
               const char* data,
               size_t data_len);

// lee “size” bytes de <filename> desde offset e imprime el resultado
err_t fs_read(file_system_t* fs,
              const char* filename,
              size_t offset,
              size_t size);

// lista todos los archivos (nombre y tamaño)
err_t fs_list(file_system_t* fs);


#endif  // FS_H
