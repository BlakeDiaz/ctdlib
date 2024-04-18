#ifndef CTD_ERROR_MESSAGE_H
#define CTD_ERROR_MESSAGE_H

#include <stdio.h>

typedef enum
{
    NO_ERROR,
    FILE_IO,
    INVALID_ARGUMENT,
    ALLOCATION_FAIL,
    MISC_ERROR,
} ERROR_TYPE;

typedef struct ctd_error
{
    ERROR_TYPE error_type;
    const char* error_message;
} ctd_error;

#define CTD_ERROR_CHECK(error)                                                                                             \
    if (error.error_type != NO_ERROR)                                                                                  \
    {                                                                                                                  \
        printf("%s\n", error.error_message);                                                                           \
    }
#define CTD_ERROR_ABORT(error) \
    if (error.error_type != NO_ERROR) \
    {                         \
        fprintf(stderr, "%s\n", error.error_message); \
        abort();\
    }

#define CTD_ERROR_PTR_ABORT(error_ptr) \
    if (error->error_type != NO_ERROR) \
    {                         \
        fprintf(stderr, "%s\n", error->error_message); \
        abort();\
    }
#endif // CTD_ERROR_MESSAGE_H
