//

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "fs.h"

typedef enum {
  CMD_UNKNOWN,
  CMD_CREATE,
  CMD_DELETE,
  CMD_WRITE,
  CMD_READ,
  CMD_LIST
} cmd_t;

err_t parser_start(file_system_t* fs, FILE* file);

err_t parser_execute(file_system_t* fs, char* buffer);

err_t parser_create(file_system_t* fs);

err_t parser_delete(file_system_t* fs);

err_t parser_write(file_system_t* fs);

err_t parser_read(file_system_t* fs);

err_t parser_list(file_system_t* fs);

#endif
