#ifndef ARG_M_H
#define ARG_M_H

enum argm_error
{
        ARGM_OK,
        ARGM_MEM_ERR,
        ARGM_NULL_ARG,
        ARGM_INVALID_ARG,
};

const struct command **
argm(int *error, const char *arg);

#endif //ARG_M_H
