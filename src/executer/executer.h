#ifndef SAPPER_H
#define SAPPER_H

#include "fs.h"

enum execute_error
{
        EXECUTOR_OK,
        EXECUTOR_ERR_BAD_ARG,
        EXECUTOR_ERR_MKDIR,
        EXECUTOR_ERR_FILE_EXISTS,
        EXECUTOR_ERR_MV,
        EXECUTOR_ERR_CREATE_PATH,
};

int
execute(int* error, const struct target *target);

#endif //SAPPER_H
