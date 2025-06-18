//

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"

// Quita el BOM UTF-8 si está al inicio de buffer
static void strip_bom(char* buffer) {
  unsigned char *ub = (unsigned char*)buffer;
  if (ub[0]==0xEF && ub[1]==0xBB && ub[2]==0xBF) {
    size_t len = strlen(buffer+3);
    memmove(buffer, buffer+3, len+1);
  }
}


#define MAX_BUFFER_LENGTH 256
#define MAX_FILENAME_LENGTH 32
#define MAX_DATA_LENGTH 64

err_t parser_start(file_system_t* fs, FILE* file) {
  char buffer[MAX_BUFFER_LENGTH];

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    // 1) Limpia BOM si existe
    strip_bom(buffer);

    // 2) Corta comentarios inline (‘#’) si los hubiera
    char* comment = strchr(buffer, '#');
    if (comment) *comment = '\0';

    // 3) Elimina CR/LF
    buffer[strcspn(buffer, "\r\n")] = '\0';

    // 4) Si quedó vacía, sigue con la siguiente línea
    if (buffer[0] == '\0') continue;

    // 5) Ejecuta el comando
    err_t err = parser_execute(fs, buffer);
    if (err != OK) {
      // ¡Sin return! Solo reportamos y continuamos
      fprintf(stderr, "Warning: línea “%s” devolvió código %d, sigo...\n",
              buffer, err);
      continue;
    }
  }

  return OK;
}



err_t parser_execute(file_system_t* fs, char* buffer) {
  char buffer_copy[MAX_BUFFER_LENGTH];
  strncpy(buffer_copy, buffer, MAX_BUFFER_LENGTH);
  buffer_copy[MAX_BUFFER_LENGTH-1] = '\0';

  char* token = strtok(buffer_copy, " ");
  if (!token) {
    fprintf(stderr, "Error: Empty command.\n");
    return ERR_CMD_EMPTY;
  }
  printf("[DEBUG] parser_execute: token='%s'\n", token);

  if (strcmp(token, "CREATE")==0) {
    printf("[DEBUG] CMD_CREATE detectado\n");
    return parser_create(fs);
  }
  if (strcmp(token, "DELETE")==0) {
    printf("[DEBUG] CMD_DELETE detectado\n");
    return parser_delete(fs);
  }
  if (strcmp(token, "WRITE")==0) {
    printf("[DEBUG] CMD_WRITE detectado\n");
    return parser_write(fs, buffer);
  }
  if (strcmp(token, "READ")==0) {
    printf("[DEBUG] CMD_READ detectado\n");
    return parser_read(fs, buffer);
  }
  if (strcmp(token, "LIST")==0) {
    printf("[DEBUG] CMD_LIST detectado\n");
    return parser_list(fs);
  }

  fprintf(stderr, "Unknown command: %s\n", token);
  return ERR_CMD_UNKNOWN;
}


err_t parser_create(file_system_t* fs) {
  char filename[MAX_FILENAME_LENGTH];

  // Obtiene siguiente token como nombre de archivo
  char* token = strtok(NULL, " ");
  if (token == NULL) {
    fprintf(stderr, "CREATE: No filename.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  strncpy(filename, token, MAX_FILENAME_LENGTH - 1);
  filename[MAX_FILENAME_LENGTH - 1] = '\0';

  // Obtiene siguiente token como tamaño de archivo
  token = strtok(NULL, " ");
  if (token == NULL) {
    fprintf(stderr, "CREATE: No size.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  // Convierte string a integer
  size_t size = strtoul(token, NULL, 10);

  // Llama a la función de creación de archivo del sistema
  fs_create(fs, filename, size);

  return OK;
}

err_t parser_delete(file_system_t* fs) {
  char filename[MAX_FILENAME_LENGTH];

  // Obtiene nombre de archivo a eliminar
  char* token = strtok(NULL, " ");
  if (token == NULL) {
    fprintf(stderr, "DELETE: No filename.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  strncpy(filename, token, MAX_FILENAME_LENGTH - 1);
  filename[MAX_FILENAME_LENGTH - 1] = '\0';

  // Elimina el archivo en el sistema de archivos
  fs_delete(fs, filename);

  return OK;
}

// WRITE <name> <offset> <data...>
err_t parser_write(file_system_t* fs, char* buffer) {
  // 1) Copia la línea para tokenizar sin alterar buffer
  char buf[MAX_BUFFER_LENGTH];
  strncpy(buf, buffer, MAX_BUFFER_LENGTH - 1);
  buf[MAX_BUFFER_LENGTH - 1] = '\0';

  // 2) Extrae tokens: comando (lo descartamos), nombre y offset
  strtok(buf, " ");                   // descarta "WRITE"
  char* name = strtok(NULL, " ");
  if (!name) {
    fprintf(stderr, "WRITE: No filename.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  char* off_s = strtok(NULL, " ");
  if (!off_s) {
    fprintf(stderr, "WRITE: No offset.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  size_t offset = strtoul(off_s, NULL, 10);

  // 3) Localiza el inicio de 'data' en la línea original
  char* p = strstr(buffer, off_s);
  if (!p) {
    fprintf(stderr, "WRITE: Offset token not found in buffer.\n");
    return ERR_CMD_UNKNOWN;
  }
  p += strlen(off_s);       // avanza tras el número
  if (*p == ' ') p++;       // salta el espacio
  const char* data = p;
  size_t data_len = strlen(data);

  // 4) Llama al FS
  return fs_write(fs, name, offset, data, data_len);
}

// READ <name> <offset> <size>
err_t parser_read(file_system_t* fs, char* buffer) {
  // 1) Copia la línea para tokenizar
  char buf[MAX_BUFFER_LENGTH];
  strncpy(buf, buffer, MAX_BUFFER_LENGTH - 1);
  buf[MAX_BUFFER_LENGTH - 1] = '\0';

  // 2) Extrae tokens: comando (descartado), nombre, offset y size
  strtok(buf, " ");                   // descarta "READ"
  char* name = strtok(NULL, " ");
  if (!name) {
    fprintf(stderr, "READ: No filename.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  char* off_s = strtok(NULL, " ");
  if (!off_s) {
    fprintf(stderr, "READ: No offset.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  char* size_s = strtok(NULL, " ");
  if (!size_s) {
    fprintf(stderr, "READ: No size.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  size_t offset = strtoul(off_s, NULL, 10);
  size_t size   = strtoul(size_s, NULL, 10);

  // 3) Llama al FS
  return fs_read(fs, name, offset, size);
}


// LIST
err_t parser_list(file_system_t* fs) {
  return fs_list(fs);
}
