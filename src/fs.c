//

#include "fs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOT_FOUND_INDEX ((size_t) -1)

size_t find_file(const file_system_t* fs, const char* name) {
  // Busca el archivo por nombre en la lista de archivos usados
  for (size_t i = 0; i < MAX_FILES; i++) {
    if (fs->files[i].in_use && strcmp(fs->files[i].name, name) == 0) {
      // Archivo encontrado, devuelve índice
      return i;
    }
  }

  // Si no se encuentra, devuelve índice especial NOT_FOUND_INDEX
  return NOT_FOUND_INDEX;
}

err_t fs_init(file_system_t* fs) {
  // Inicializa el sistema de archivos: reserva memoria para estructuras
  fs->files = malloc(MAX_FILES * sizeof(file_t));
  if (fs->files == NULL) {
    return ERR_NO_MEMORY;
  }

  // Reserva espacio para los datos de archivos (bloques de datos)
  fs->data_blocks = malloc(TOTAL_BLOCKS);
  if (fs->data_blocks == NULL) {
    free(fs->files);
    fs->files = NULL;

    return ERR_NO_MEMORY;
  }

  // Reserva mapa de uso de bloques (libre u ocupado)
  fs->block_map = malloc(TOTAL_BLOCKS * sizeof(bool));
  if (fs->block_map == NULL) {
    free(fs->files);
    fs->files = NULL;

    free(fs->data_blocks);
    fs->data_blocks = NULL;

    return ERR_NO_MEMORY;
  }

  // Inicializa cada entrada de archivo como no en uso
  for (size_t i = 0; i < MAX_FILES; i++) {
    fs->files[i].in_use = false;
     // Sin bloques asignados
    fs->files[i].used_blocks = NULL;
  }

  // Marca todos los bloques de datos como libres
  for (size_t i = 0; i < TOTAL_BLOCKS; i++) {
    fs->block_map[i] = false;
  }

  return OK;
}

err_t fs_create(file_system_t* fs, const char* name, size_t size) {
  // Comprueba si ya existe un archivo con el mismo nombre
  if (find_file(fs, name) != NOT_FOUND_INDEX) {
    fprintf(stderr, "Error: file '%s' already exists.\n", name);
    return ERR_FILE_ALREADY_EXISTS;
  }

  // Busca un índice libre para el nuevo archivo
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

  // Calcula número de bloques necesarios para el tamaño solicitado
  size_t blocks_needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

  file_t* file = &fs->files[file_index];

  // Reserva memoria para la lista de bloques que usará el archivo
  file->used_blocks = (size_t*) malloc(blocks_needed * sizeof(size_t));
  if (file->used_blocks == NULL) {
    return ERR_NO_MEMORY;
  }

  file->block_count = 0;

  size_t blocks_used = 0;

  // Asigna bloques libres hasta cubrir la cantidad necesaria
  for (size_t i = 0; i < TOTAL_BLOCKS && blocks_used < blocks_needed; i++) {
    if (!fs->block_map[i]) {
      // Marca bloque como ocupado
      fs->block_map[i] = true;
      // Guarda índice de bloque
      file->used_blocks[blocks_used] = i;
      blocks_used++;
    }
  }

  // Si no se pudo asignar todo el espacio, revierte asignaciones
  if (blocks_used < blocks_needed) {
    for (size_t i = 0; i < blocks_used; i++) {
      // Libera bloques asignados
      fs->block_map[file->used_blocks[i]] = false;
    }

    free(file->used_blocks);
    file->used_blocks = NULL;

    fprintf(stderr, "Error: Not enough space to create file '%s'.\n", name);
    return ERR_NOT_ENOUGH_SPACE;
  }

  // Configura metadatos del archivo
  file->in_use = true;
  strncpy(file->name, name, MAX_FILENAME - 1);
  file->name[MAX_FILENAME - 1] = '\0';
  file->size = size;
  file->block_count = blocks_needed;

  printf(
    "Info: File '%s' created with %zu blocks.\n",
    file->name,
    file->block_count
  );

  return OK;
}

err_t fs_delete(file_system_t* fs, const char* name) {
  // Busca el índice del archivo a eliminar
  size_t file_index = find_file(fs, name);

  if (file_index == NOT_FOUND_INDEX) {
    fprintf(stderr, "Error: File '%s' not found.\n", name);
    return ERR_FILE_NOT_FOUND;
  }

  file_t* file = &fs->files[file_index];

  // Libera todos los bloques asignados al archivo
  for (size_t i = 0; i < file->block_count; i++) {
    fs->block_map[file->used_blocks[i]] = false;
  }

  // Marca archivo como no en uso
  file->in_use = false;

  // Libera lista de bloques
  free(file->used_blocks);
  file->used_blocks = NULL;

  printf("Info: File '%s' deleted.\n", file->name);

  return OK;
}

void fs_free(file_system_t* fs) {
  // Libera todos los recursos del sistema de archivos
  if (fs == NULL) {
    return;
  }

  // Libera memoria de bloques usados por cada archivo activo
  for (size_t i = 0; i < MAX_FILES; i++) {
    if (fs->files[i].in_use && fs->files[i].used_blocks != NULL) {
      free(fs->files[i].used_blocks);
    }
  }

  // Libera arreglo de archivos
  if (fs->files != NULL) {
    free(fs->files);
    fs->files = NULL;
  }

  // Libera bloques de datos
  if (fs->data_blocks != NULL) {
    free(fs->data_blocks);
    fs->data_blocks = NULL;
  }

  // Libera mapa de bloques
  if (fs->block_map != NULL) {
    free(fs->block_map);
    fs->block_map = NULL;
  }
}
