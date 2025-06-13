//

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fs.h"

#define MAX_BUFFER_LENGTH 256
#define MAX_FILENAME_LENGTH 32
#define MAX_DATA_LENGTH 64

err_t parser_start(file_system_t* fs, FILE* file) {
  char buffer[MAX_BUFFER_LENGTH];

  while (fgets(buffer, sizeof(buffer), file) != NULL) {
    if (buffer[0] == '\0' && buffer[0] == '\n') {
      buffer[strcspn(buffer, "\n")] = 0;

      if (strlen(buffer) > 0) {
        err_t error = parser_execute(fs, buffer);
        if (error != OK) {
          return error;
        }
      }
    }
  }

  return OK;
}

err_t parser_execute(file_system_t* fs, char* buffer) {
  char command[MAX_BUFFER_LENGTH];

  char buffer_copy[MAX_BUFFER_LENGTH];
  strncpy(buffer_copy, buffer, MAX_BUFFER_LENGTH - 1);
  buffer_copy[MAX_BUFFER_LENGTH - 1] = '\0';

  char* token = strtok(buffer_copy, " ");
  if (token == NULL) {
    fprintf(stderr, "Error: Empty command.\n");
    return ERR_CMD_EMPTY;
  }
  strncpy(command, token, MAX_BUFFER_LENGTH - 1);
  command[MAX_BUFFER_LENGTH - 1] = '\0';

  cmd_t cmd_type = CMD_UNKNOWN;

  if (strcmp(command, "CREATE") == 0) {
    cmd_type = CMD_CREATE;
  } else if (strcmp(command, "DELETE") == 0) {
    cmd_type = CMD_DELETE;
  } else if (strcmp(command, "WRITE") == 0) {
    cmd_type = CMD_WRITE;
  } else if (strcmp(command, "READ") == 0) {
    cmd_type = CMD_READ;
  } else if (strcmp(command, "LIST") == 0) {
    cmd_type = CMD_LIST;
  }

  switch (cmd_type) {
    case CMD_CREATE:
      return parser_create(fs, buffer_copy);
    case CMD_DELETE:
      return parser_delete(fs, buffer_copy);
    case CMD_WRITE:
      return parser_write(fs, buffer_copy);
    case CMD_READ:
      return parser_read(fs, buffer_copy);
    case CMD_LIST:
      return parser_list(fs, buffer_copy);
    default:
      fprintf(stderr, "Unknown command: %s\n", command);
      return ERR_CMD_UNKNOWN;
  }
}

err_t parser_create(file_system_t* fs, char* buffer) {
  char filename[MAX_FILENAME_LENGTH];

  char* token = strtok(NULL, " ");
  if (token == NULL) {
    fprintf(stderr, "CREATE: No filename.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  strncpy(filename, token, MAX_FILENAME_LENGTH - 1);
  filename[MAX_FILENAME_LENGTH - 1] = '\0';

  token = strtok(NULL, " ");
  if (token == NULL) {
    fprintf(stderr, "CREATE: No size.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  size_t size = atoi(token);

  fs_create(fs, filename, size);

  return OK;
}

err_t parser_delete(file_system_t* fs, char* buffer) {
  char filename[MAX_FILENAME_LENGTH];

  char* token = strtok(NULL, " ");
  if (token == NULL) {
    fprintf(stderr, "DELETE: No filename.\n");
    return ERR_CMD_NO_ARGUMENT;
  }
  strncpy(filename, token, MAX_FILENAME_LENGTH - 1);
  filename[MAX_FILENAME_LENGTH - 1] = '\0';

  fs_delete(fs, filename);

  return OK;
}

err_t parser_write(file_system_t* fs, char* buffer) {
  // fs_write();
  (void) fs;
  (void) buffer;
  fprintf(stderr, "WRITE: Not implemented.\n");
  return ERR_CMD_UNKNOWN;
}

err_t parser_read(file_system_t* fs, char* buffer) {
  // fs_read();
  (void) fs;
  (void) buffer;
  fprintf(stderr, "READ: Not implemented.\n");
  return ERR_CMD_UNKNOWN;
}

err_t parser_list(file_system_t* fs, char* buffer) {
  // fs_list();
  (void) fs;
  (void) buffer;
  fprintf(stderr, "LIST: Not implemented.\n");
  return ERR_CMD_UNKNOWN;
}
