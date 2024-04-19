#ifndef CTD_GENERIC_DYNAMIC_ARRAY_C
#define CTD_GENERIC_DYNAMIC_ARRAY_C
#include <ctd_error.h>
#include <ctd_define.h>
#include <ctd_macro_tools.h>
#include <stdlib.h>

#define CTD_DEFAULT_DYNAMIC_ARRAY_TYPES(X)                                                                             \
    X(bool, bool)                                                                                                      \
    X(char, char)                                                                                                      \
    X(signed char, schar)                                                                                              \
    X(unsigned char, uchar)                                                                                            \
    X(short, short)                                                                                                    \
    X(signed short, sshort)                                                                                            \
    X(unsigned short, ushort)                                                                                          \
    X(int, int)                                                                                                        \
    X(unsigned int, uint)                                                                                              \
    X(long, long)                                                                                                      \
    X(signed long, slong)                                                                                              \
    X(unsigned long, ulong)                                                                                            \
    X(size_t, size_t)                                                                                                  \
    X(float, float)                                                                                                    \
    X(double, double)                                                                                                  \
    X(long double, ldouble)                                                                                            \
    X(uint8_t, uint8_t)                                                                                                \
    X(uint16_t, uint16_t)                                                                                              \
    X(uint32_t, uint32_t)                                                                                              \
    X(uint64_t, uint64_t)                                                                                              \
    X(uint_fast8_t, uint_fast8_t)                                                                                      \
    X(uint_fast16_t, uint_fast16_t)                                                                                    \
    X(uint_fast32_t, uint_fast32_t)                                                                                    \
    X(uint_fast64_t, uint_fast64_t)                                                                                    \
    X(uint_least8_t, uint_least8_t)                                                                                    \
    X(uint_least16_t, uint_least16_t)                                                                                  \
    X(uint_least32_t, uint_least32_t)                                                                                  \
    X(uint_least64_t, uint_least64_t)                                                                                  \
    X(uintmax_t, uintmax_t)                                                                                            \
    X(uintptr_t, uintptr_t)                                                                                            \
    X(int8_t, int8_t)                                                                                                  \
    X(int16_t, int16_t)                                                                                                \
    X(int32_t, int32_t)                                                                                                \
    X(int64_t, int64_t)                                                                                                \
    X(int_fast8_t, int_fast8_t)                                                                                        \
    X(int_fast16_t, int_fast16_t)                                                                                      \
    X(int_fast32_t, int_fast32_t)                                                                                      \
    X(int_fast64_t, int_fast64_t)                                                                                      \
    X(int_least8_t, int_least8_t)                                                                                      \
    X(int_least16_t, int_least16_t)                                                                                    \
    X(int_least32_t, int_least32_t)                                                                                    \
    X(int_least64_t, int_least64_t)                                                                                    \
    X(intmax_t, intmax_t)                                                                                              \
    X(intptr_t, intptr_t)

#define FORMAT_ARRAY(type, typename, ...) , ctd_dynamic_array_##typename* : ctd_dynamic_array_##typename##_##__VA_ARGS__

#define ctd_dynamic_array(typename) ctd_dynamic_array_##typename

#define ctd_dynamic_array_create(typename, capacity, error_ptr)                                                        \
    ctd_dynamic_array_##typename##_create((capacity), (error_ptr))

#define ctd_dynamic_array_initialize(typename, variable_name, capacity, error_ptr)                                     \
    ctd_dynamic_array_##typename variable_name = ctd_dynamic_array_##typename##_create((capacity), (error_ptr))

#define ctd_dynamic_array_destroy(ctd_dynamic_array, error_ptr)                                                        \
    _Generic((ctd_dynamic_array),                                                                                      \
        REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, destroy)))(ctd_dynamic_array, error_ptr)

#define ctd_dynamic_array_append(ctd_dynamic_array, item, error_ptr)                                                   \
    _Generic((ctd_dynamic_array),                                                                                      \
        REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, append)))(ctd_dynamic_array, item, error_ptr)

#define ctd_dynamic_array_append_ctd_dynamic_array(ctd_dynamic_array, to_append, error_ptr)                            \
    _Generic((ctd_dynamic_array),                                                                                      \
        REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, append_ctd_dynamic_array)))(ctd_dynamic_array,      \
                                                                                               to_append, error_ptr)

#define ctd_dynamic_array_insert(ctd_dynamic_array, item, error_ptr)                                                   \
    _Generic((ctd_dynamic_array),                                                                                      \
        REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, insert)))(ctd_dynamic_array, item, error_ptr)

#define ctd_dynamic_array_insert_ctd_dynamic_array(ctd_dynamic_array, to_insert, index, error_ptr)                     \
    _Generic((ctd_dynamic_array),                                                                                      \
        REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, insert_ctd_dynamic_array)))(                        \
        ctd_dynamic_array, to_insert, index, error_ptr)

#define ctd_dynamic_array_pop_back(ctd_dynamic_array, error_ptr)                                                       \
    _Generic((ctd_dynamic_array),                                                                                      \
        REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, pop_back)))(ctd_dynamic_array, error_ptr)

#define ctd_dynamic_array_remove(ctd_dynamic_array, index, error_ptr)                                                  \
    _Generic((ctd_dynamic_array),                                                                                      \
        REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, remove)))(ctd_dynamic_array, index, error_ptr)

#define ctd_dynamic_array_remove_multiple(ctd_dynamic_array, first_index, last_index, error_ptr)                       \
    _Generic((ctd_dynamic_array), REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, remove_multiple)))(       \
        ctd_dynamic_array, first_index, last_index, error_ptr)

#define ctd_dynamic_array_clear(ctd_dynamic_array, error_ptr)                                                          \
    _Generic((ctd_dynamic_array),                                                                                      \
        REMOVE_LEADING_COMMA(CTD_DYNAMIC_ARRAY_TYPES(FORMAT_ARRAY, clear)))(ctd_dynamic_array, error_ptr)

/*
#define ctd_dynamic_array_begin(ctd_dynamic_array) ctd_dynamic_array.data

#define ctd_dynamic_array_end(ctd_dynamic_array) ctd_dynamic_array->data + ctd_dynamic_array->length
*/

#define CTD_DYNAMIC_ARRAY_TYPE_DECL(type, typename, ...)                                                               \
    typedef struct ctd_dynamic_array_##typename                                                                        \
    {                                                                                                                  \
        ptrdiff_t length;                                                                                              \
        ptrdiff_t capacity;                                                                                            \
        type* data;                                                                                                    \
    }                                                                                                                  \
    ctd_dynamic_array_##typename;                                                                                      \
    typedef struct ctd_dynamic_array_##typename##_iterator                                                             \
    {                                                                                                                  \
        type* ptr;                                                                                                     \
    }                                                                                                                  \
    ctd_dynamic_array_##typename##_iterator;

#define CTD_DYNAMIC_ARRAY_FUNCTIONS_DECL(type, typename, ...)                                                          \
    ctd_dynamic_array_##typename ctd_dynamic_array_##typename##_create(ptrdiff_t capacity, ctd_error* error);          \
    void ctd_dynamic_array_##typename##_destroy(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error);    \
    void ctd_dynamic_array_##                                                                                          \
        typename##_append(ctd_dynamic_array_##typename* ctd_dynamic_array, type item, ctd_error* error);               \
    void ctd_dynamic_array_##typename##_append_ctd_dynamic_array(                                                      \
        ctd_dynamic_array_##typename* ctd_dynamic_array, const ctd_dynamic_array_##typename to_append, ctd_error* error);  \
    void ctd_dynamic_array_##typename##_insert(ctd_dynamic_array_##typename* ctd_dynamic_array, type item,             \
                                               const ptrdiff_t index, ctd_error* error);                                   \
    void ctd_dynamic_array_##typename##_insert_ctd_dynamic_array(ctd_dynamic_array_##typename* ctd_dynamic_array,      \
                                                                 const ctd_dynamic_array_##typename to_insert,         \
                                                                 const ptrdiff_t index, ctd_error* error);             \
    type ctd_dynamic_array_##typename##_pop_back(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error);   \
    type ctd_dynamic_array_##                                                                                          \
        typename##_remove(ctd_dynamic_array_##typename* ctd_dynamic_array, const ptrdiff_t index, ctd_error* error);   \
    type ctd_dynamic_array_##                                                                                          \
        typename##_remove_multiple(ctd_dynamic_array_##typename* ctd_dynamic_array, const ptrdiff_t first_index,       \
                                   const ptrdiff_t last_index, ctd_error* error);                                      \
    void ctd_dynamic_array_##typename##_clear(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error);      \
    ctd_dynamic_array_##typename##_iterator ctd_dynamic_array_##                                                       \
        typename##_iterator_begin(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error);                  \
    ctd_dynamic_array_##typename##_iterator ctd_dynamic_array_##                                                       \
        typename##_iterator_end(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error);                    \
    void ctd_dynamic_array_##                                                                                          \
        typename##_iterator_increment(ctd_dynamic_array_##typename##_iterator* iterator, ctd_error* error);

#define DYNAMIC_ARRAY_IMPL(type, typename, ...)                                                                        \
    void ctd_dynamic_array_##typename##_resize(ctd_dynamic_array_##typename* ctd_dynamic_array,                        \
                                               const ptrdiff_t new_capacity, ctd_error* error);                        \
    void ctd_dynamic_array_##typename##_maybe_expand(ctd_dynamic_array_##typename* ctd_dynamic_array,                  \
                                                     const ptrdiff_t expand_by, ctd_error* error);                     \
    void ctd_dynamic_array_##typename##_maybe_contract(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error); \
    ctd_dynamic_array_##typename ctd_dynamic_array_##typename##_create(ptrdiff_t capacity, ctd_error* error)           \
    {                                                                                                                  \
        if (capacity == 0)                                                                                             \
        {                                                                                                              \
            capacity = 1;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array_##typename ctd_dynamic_array = {0};                                                          \
        ctd_dynamic_array.length = 0;                                                                                  \
        ctd_dynamic_array.capacity = capacity;                                                                         \
        ctd_dynamic_array.data = malloc(sizeof(type) * capacity);                                                      \
        if (ctd_dynamic_array.data == NULL)                                                                            \
        {                                                                                                              \
            error->error_type = ALLOCATION_FAIL;                                                                       \
            error->error_message = "Malloc Failed.";                                                                   \
                                                                                                                       \
            ctd_dynamic_array_##typename empty = {0};                                                                  \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return ctd_dynamic_array;                                                                                      \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_dynamic_array_##typename##_destroy(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error)     \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
        if (ctd_dynamic_array->data != NULL)                                                                           \
        {                                                                                                              \
            free(ctd_dynamic_array->data);                                                                             \
        }                                                                                                              \
        ctd_dynamic_array_##typename empty = {0};                                                                      \
        *ctd_dynamic_array = empty;                                                                                    \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_dynamic_array_##                                                                                          \
        typename##_append(ctd_dynamic_array_##typename* ctd_dynamic_array, type item, ctd_error* error)                \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array_##typename##_maybe_expand(ctd_dynamic_array, 1, error);                                      \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array->data[ctd_dynamic_array->length] = item;                                                     \
        ctd_dynamic_array->length++;                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_dynamic_array_##typename##_append_ctd_dynamic_array(ctd_dynamic_array_##typename* ctd_dynamic_array,      \
                                                                 ctd_dynamic_array_##typename to_append, ctd_error* error) \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array_##typename##_maybe_expand(ctd_dynamic_array, to_append.length, error);                       \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        ptrdiff_t to_append_index = 0;                                                                                 \
        ptrdiff_t ctd_dynamic_array_index = ctd_dynamic_array->length;                                                 \
                                                                                                                       \
        while (to_append_index < to_append.length)                                                                     \
        {                                                                                                              \
            ctd_dynamic_array->data[ctd_dynamic_array_index] = to_append.data[to_append_index];                        \
                                                                                                                       \
            to_append_index++;                                                                                         \
            ctd_dynamic_array_index++;                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array->length += to_append.length;                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_dynamic_array_##typename##_insert(ctd_dynamic_array_##typename* ctd_dynamic_array, type item,             \
                                               const ptrdiff_t index, ctd_error* error)                                \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "String to append to was null";                                                     \
            return;                                                                                                    \
        }                                                                                                              \
        if (index > ctd_dynamic_array->length || index < 0)                                                            \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Invalid insert index";                                                             \
            return;                                                                                                    \
        }                                                                                                              \
        if (index == ctd_dynamic_array->length)                                                                        \
        {                                                                                                              \
            ctd_dynamic_array_##typename##_append(ctd_dynamic_array, item, error);                                     \
            if (error->error_type != NO_ERROR)                                                                         \
            {                                                                                                          \
                return;                                                                                                \
            }                                                                                                          \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array_##typename##_maybe_expand(ctd_dynamic_array, 1, error);                                      \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        for (ptrdiff_t i = ctd_dynamic_array->length - 1; i >= index; i--)                                             \
        {                                                                                                              \
            ctd_dynamic_array->data[i + 1] = ctd_dynamic_array->data[i];                                               \
        }                                                                                                              \
        ctd_dynamic_array->data[index] = item;                                                                         \
                                                                                                                       \
        ctd_dynamic_array->length++;                                                                                   \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_dynamic_array_##typename##_insert_ctd_dynamic_array(ctd_dynamic_array_##typename* ctd_dynamic_array,      \
                                                                 const ctd_dynamic_array_##typename to_insert,         \
                                                                 const ptrdiff_t index, ctd_error* error)              \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "String to append to was null";                                                     \
            return;                                                                                                    \
        }                                                                                                              \
        if (index > ctd_dynamic_array->length || index < 0)                                                            \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Invalid insert index";                                                             \
            return;                                                                                                    \
        }                                                                                                              \
        if (index == ctd_dynamic_array->length)                                                                        \
        {                                                                                                              \
            ctd_dynamic_array_##typename##_append_ctd_dynamic_array(ctd_dynamic_array, to_insert, error);              \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        ptrdiff_t new_length = ctd_dynamic_array->length + to_insert.length;                                           \
        ctd_dynamic_array_##typename##_maybe_expand(ctd_dynamic_array, to_insert.length, error);                       \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        for (ptrdiff_t i = ctd_dynamic_array->length - 1; i >= index; i--)                                             \
        {                                                                                                              \
            ctd_dynamic_array->data[i + 1] = ctd_dynamic_array->data[i];                                               \
        }                                                                                                              \
                                                                                                                       \
        ptrdiff_t to_insert_index = 0;                                                                                 \
        ptrdiff_t ctd_dynamic_array_index = index;                                                                     \
        while (to_insert_index < to_insert.length)                                                                     \
        {                                                                                                              \
            ctd_dynamic_array->data[ctd_dynamic_array_index] = to_insert.data[to_insert_index];                        \
                                                                                                                       \
            to_insert_index++;                                                                                         \
            ctd_dynamic_array_index++;                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array->length = new_length;                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    type ctd_dynamic_array_##typename##_pop_back(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error)    \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
        if (ctd_dynamic_array->length == 0)                                                                            \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was empty";                                                                  \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        type popped_element = ctd_dynamic_array->data[ctd_dynamic_array->length - 1];                                  \
                                                                                                                       \
        ctd_dynamic_array->length--;                                                                                   \
                                                                                                                       \
        ctd_dynamic_array_##typename##_maybe_contract(ctd_dynamic_array, error);                                       \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return popped_element;                                                                                         \
    }                                                                                                                  \
                                                                                                                       \
    type ctd_dynamic_array_##                                                                                          \
        typename##_remove(ctd_dynamic_array_##typename* ctd_dynamic_array, const ptrdiff_t index, ctd_error* error)    \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
        if (index < 0 || index >= ctd_dynamic_array->length)                                                           \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Index out of range.";                                                              \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        type removed_element = ctd_dynamic_array->data[index];                                                         \
                                                                                                                       \
        for (ptrdiff_t i = index; i < ctd_dynamic_array->length - 1; i++)                                              \
        {                                                                                                              \
            ctd_dynamic_array[i] = ctd_dynamic_array[i + 1];                                                           \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array->length--;                                                                                   \
                                                                                                                       \
        ctd_dynamic_array_##typename##_maybe_contract(ctd_dynamic_array, error);                                       \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return removed_element;                                                                                        \
    }                                                                                                                  \
                                                                                                                       \
    type ctd_dynamic_array_##                                                                                          \
        typename##_remove_multiple(ctd_dynamic_array_##typename* ctd_dynamic_array, const ptrdiff_t first_index,       \
                                   const ptrdiff_t last_index, ctd_error* error)                                       \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
        if (first_index < 0 || first_index >= ctd_dynamic_array->length)                                               \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "First index out of range.";                                                        \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
        if (last_index < 0 || last_index >= ctd_dynamic_array->length)                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Last index out of range.";                                                         \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
        if (first_index > last_index)                                                                                  \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "First index greater than last index.";                                             \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
        if (first_index == last_index)                                                                                 \
        {                                                                                                              \
            return ctd_dynamic_array_##typename##_remove(ctd_dynamic_array, first_index, error);                       \
        }                                                                                                              \
        const ptrdiff_t number_of_elements_subtracted = (last_index - first_index) + 1;                                \
                                                                                                                       \
        type last_removed_element = ctd_dynamic_array->data[last_index];                                               \
                                                                                                                       \
        for (ptrdiff_t i = first_index; i < ctd_dynamic_array->length - 1 - number_of_elements_subtracted; i++)        \
        {                                                                                                              \
            ctd_dynamic_array[i] = ctd_dynamic_array[i + number_of_elements_subtracted];                               \
        }                                                                                                              \
                                                                                                                       \
        ctd_dynamic_array->length -= number_of_elements_subtracted;                                                    \
                                                                                                                       \
        ctd_dynamic_array_##typename##_maybe_contract(ctd_dynamic_array, error);                                       \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return last_removed_element;                                                                                   \
    }                                                                                                                  \
    void ctd_dynamic_array_##typename##_clear(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error)       \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        type empty = {0};                                                                                              \
        for (ptrdiff_t i = 0; i < ctd_dynamic_array->length; i++)                                                      \
        {                                                                                                              \
            ctd_dynamic_array->data[i] = empty;                                                                        \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_dynamic_array_##                                                                                          \
        typename##_resize(ctd_dynamic_array_##typename* ctd_dynamic_array, const ptrdiff_t new_capacity, ctd_error* error) \
    {                                                                                                                  \
        type* new_data = realloc(ctd_dynamic_array->data, new_capacity * sizeof(type));                                \
        if (new_data == NULL)                                                                                          \
        {                                                                                                              \
            error->error_type = ALLOCATION_FAIL;                                                                       \
            error->error_message = "Realloc Failed.";                                                                  \
            return;                                                                                                    \
        }                                                                                                              \
        ctd_dynamic_array->capacity = new_capacity;                                                                    \
        ctd_dynamic_array->data = new_data;                                                                            \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_dynamic_array_##typename##_maybe_expand(ctd_dynamic_array_##typename* ctd_dynamic_array,                  \
                                                     const ptrdiff_t expand_by, ctd_error* error)                      \
    {                                                                                                                  \
        if (ctd_dynamic_array->length + expand_by >= ctd_dynamic_array->capacity)                                      \
        {                                                                                                              \
            ctd_dynamic_array_##typename##_resize(                                                                     \
                ctd_dynamic_array, max(ctd_dynamic_array->length + expand_by, ctd_dynamic_array->capacity * 2 + 1),    \
                error);                                                                                                \
        }                                                                                                              \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_dynamic_array_##typename##_maybe_contract(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error)  \
    {                                                                                                                  \
        if (ctd_dynamic_array->length < ctd_dynamic_array->capacity / 4)                                               \
        {                                                                                                              \
            ctd_dynamic_array_##typename##_resize(ctd_dynamic_array, ctd_dynamic_array->capacity / 2, error);          \
        }                                                                                                              \
    }                                                                                                                  \
    ctd_dynamic_array_##typename##_iterator ctd_dynamic_array_##                                                       \
        typename##_iterator_begin(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error)                   \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            ctd_dynamic_array_##typename##_iterator empty = {0};                                                       \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return (ctd_dynamic_array_##typename##_iterator){.ptr = ctd_dynamic_array->data};                              \
    }                                                                                                                  \
    ctd_dynamic_array_##typename##_iterator ctd_dynamic_array_##                                                       \
        typename##_iterator_end(ctd_dynamic_array_##typename* ctd_dynamic_array, ctd_error* error)                     \
    {                                                                                                                  \
        if (ctd_dynamic_array == NULL)                                                                                 \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Array was NULL";                                                                   \
                                                                                                                       \
            ctd_dynamic_array_##typename##_iterator empty = {0};                                                       \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return (ctd_dynamic_array_##typename##_iterator){.ptr = ctd_dynamic_array->data + ctd_dynamic_array->length};  \
    }                                                                                                                  \
    void ctd_dynamic_array_##                                                                                          \
        typename##_iterator_increment(ctd_dynamic_array_##typename##_iterator* iterator, ctd_error* error)             \
    {                                                                                                                  \
        if (iterator->ptr == NULL)                                                                                     \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Iterator was NULL";                                                                \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        iterator->ptr += 1;                                                                                            \
    }
#endif // CTD_GENERIC_DYNAMIC_ARRAY_C
