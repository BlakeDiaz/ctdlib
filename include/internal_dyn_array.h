#ifndef PROGRAM_INTERNAL_DYN_ARRAY_H
#define PROGRAM_INTERNAL_DYN_ARRAY_H
#include <stddef.h>

#define ctd_internal_dynamic_array(type)                                                                               \
    struct                                                                                                             \
    {                                                                                                                  \
        type* data;                                                                                                    \
        ptrdiff_t length;                                                                                              \
        ptrdiff_t capacity;                                                                                            \
    }

#define ctd_internal_dynamic_array_append(array, type, item, error_ptr)                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        void* _new_data;                                                                                               \
        if (array.length == array.capacity)                                                                            \
        {                                                                                                              \
            array.capacity *= 2;                                                                                       \
            _new_data = realloc(array.data, array.capacity * sizeof(type));                                            \
            if (_new_data == NULL)                                                                                     \
            {                                                                                                          \
                free(array.data);                                                                                      \
                error->error_type = ALLOCATION_FAIL;                                                                   \
                error->error_message = "Realloc failed.";                                                              \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                array.data = _new_data;                                                                                \
                array.data[array.length] = item;                                                                       \
                array.length++;                                                                                        \
            }                                                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            array.data[array.length] = item;                                                                           \
            array.length++;                                                                                            \
        }                                                                                                              \
    } while (0)

#endif // PROGRAM_INTERNAL_DYN_ARRAY_H
