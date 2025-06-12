//

#ifndef ERROR_H
#define ERROR_H

typedef enum {
  OK = 0,
  ERR_FILE_ALREADY_EXISTS,
  ERR_MAX_FILES_REACHED,
  ERR_NOT_ENOUGH_SPACE,
  ERR_FILE_NOT_FOUND,
  ERR_NO_MEMORY,
} err_t;

#endif  // ERROR_H
