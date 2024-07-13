#ifndef CTD_INTERNAL_DYNAMIC_ARRAY_H
#define CTD_INTERNAL_DYNAMIC_ARRAY_H
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
                error_ptr->error_type = ALLOCATION_FAIL;                                                               \
                error_ptr->error_message = "Realloc failed.";                                                          \
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

#define ctd_internal_dynamic_array_append_with_allocator(array, type, item, allocator, error_ptr)                      \
    do                                                                                                                 \
    {                                                                                                                  \
        void* _new_data;                                                                                               \
        if ((array).length == (array).capacity)                                                                        \
        {                                                                                                              \
            _new_data = (allocator).reallocate((allocator).context, (array).data, (array).capacity * sizeof(type),     \
                                             ((array).capacity * 2 + 1) * sizeof(type), alignof(type));                \
            if (_new_data == NULL)                                                                                     \
            {                                                                                                          \
                (allocator).deallocate((allocator).context, (array).data, (array).capacity * sizeof(type));            \
                (error_ptr)->error_type = ALLOCATION_FAIL;                                                             \
                (error_ptr)->error_message = "Realloc failed.";                                                        \
            }                                                                                                          \
            else                                                                                                       \
            {                                                                                                          \
                (array).data = _new_data;                                                                              \
                (array).data[(array).length] = item;                                                                   \
                (array).length++;                                                                                      \
                (array).capacity = (array).capacity * 2 + 1;                                                           \
            }                                                                                                          \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            (array).data[(array).length] = item;                                                                       \
            (array).length++;                                                                                          \
        }                                                                                                              \
    } while (0)

#endif // CTD_INTERNAL_DYNAMIC_ARRAY_H
