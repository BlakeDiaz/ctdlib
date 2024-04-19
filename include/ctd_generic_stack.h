#ifndef CTD_GENERIC_STACK_H
#define CTD_GENERIC_STACK_H
#include <ctd_error.h>
#include <ctd_define.h>
#include <stdlib.h>

#define FORMAT_STACK(type, typename, function_name) , ctd_stack_##typename* : ctd_stack_##typename##_##function_name

#define ctd_stack(typename) ctd_stack_##typename

#define ctd_stack_create(typename, capacity, error_ptr) ctd_stack_##typename##_create((capacity), (error_ptr))

#define ctd_stack_initialize(typename, variable_name, capacity, error_ptr)                                             \
    ctd_stack_##typename variable_name = ctd_stack_##typename##_create((capacity), (error_ptr))

#define ctd_stack_destroy(stack_ptr)                                                                                   \
_Generic((stack_ptr), REMOVE_LEADING_COMMA(CTD_STACK_TYPES(FORMAT_STACK, destroy))(stack_ptr)

#define ctd_stack_push(stack_ptr, value, error_ptr)                                                                    \
_Generic((stack_ptr), REMOVE_LEADING_COMMA(CTD_STACK_TYPES(FORMAT_STACK, push))(stack_ptr, value, error_ptr)

#define ctd_stack_pop(stack_ptr, error_ptr)                                                                            \
_Generic((stack_ptr), REMOVE_LEADING_COMMA(CTD_STACK_TYPES(FORMAT_STACK, pop))(stack_ptr, error_ptr)

#define ctd_stack_peek(stack_ptr, error_ptr)                                                                           \
_Generic((stack_ptr), REMOVE_LEADING_COMMA(CTD_STACK_TYPES(FORMAT_STACK, peek))(stack_ptr, error_ptr)

#define GENERIC_STACK_TYPE_DECL(type, typename, ...)                                                                   \
    typedef struct ctd_stack_##typename                                                                                \
    {                                                                                                                  \
        type* data;                                                                                                    \
        ptrdiff_t length;                                                                                              \
        ptrdiff_t capacity;                                                                                            \
    }                                                                                                                  \
    ctd_stack_##typename;                                                                                              \
    typedef struct ctd_stack_##typename##_iterator                                                                     \
    {                                                                                                                  \
        type* ptr;                                                                                                     \
    }                                                                                                                  \
    ctd_stack_##typename##_iterator;

#define GENERIC_STACK_FUNCTIONS_DECL(type, typename, ...)                                                              \
    ctd_stack_##typename ctd_stack_##typename##_create(ptrdiff_t capacity, ctd_error* error);                          \
    void ctd_stack_##typename##_destroy(ctd_stack_##typename* self);                                                   \
    void ctd_stack_##typename##_push(ctd_stack_##typename* self, type value, ctd_error* error);                        \
    type ctd_stack_##typename##_pop(ctd_stack_##typename* self, ctd_error* error);                                     \
    type ctd_stack_##typename##_peek(ctd_stack_##typename* self, ctd_error* error);                                    \
    ctd_stack_##typename##_iterator ctd_stack_##                                                                       \
        typename##_iterator_begin(ctd_stack_##typename* ctd_stack, ctd_error* error);                                  \
    ctd_stack_##typename##_iterator ctd_stack_##                                                                       \
        typename##_iterator_end(ctd_stack_##typename* ctd_stack, ctd_error* error);                                    \
    void ctd_stack_##typename##_iterator_increment(ctd_stack_##typename##_iterator* iterator, ctd_error* error);

#define GENERIC_STACK_IMPL(type, typename, ...)                                                                        \
    void ctd_stack_##typename##_resize(ctd_stack_##typename* self, ptrdiff_t new_capacity, ctd_error* error);          \
    void ctd_stack_##typename##_maybe_expand(ctd_stack_##typename* self, ctd_error* error);                            \
    void ctd_stack_##typename##_maybe_contract(ctd_stack_##typename* self, ctd_error* error);                          \
    ctd_stack_##typename ctd_stack_##typename##_create(ptrdiff_t capacity, ctd_error* error)                           \
    {                                                                                                                  \
        if (capacity == 0)                                                                                             \
        {                                                                                                              \
            capacity = 1;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        ctd_stack_##typename stack = {0};                                                                              \
        stack.length = 0;                                                                                              \
        stack.capacity = capacity;                                                                                     \
        stack.data = malloc(sizeof(type) * stack.capacity);                                                            \
        if (stack.data == NULL)                                                                                        \
        {                                                                                                              \
            error->error_type = ALLOCATION_FAIL;                                                                       \
            error->error_message = "Malloc Failed.";                                                                   \
                                                                                                                       \
            ctd_stack_##typename empty = {0};                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return stack;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_stack_##typename##_destroy(ctd_stack_##typename* self)                                                    \
    {                                                                                                                  \
        if (self->data != NULL)                                                                                        \
        {                                                                                                              \
            free(self->data);                                                                                          \
        }                                                                                                              \
        ctd_stack_##typename empty = {0};                                                                              \
        *self = empty;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    void ctd_stack_##typename##_push(ctd_stack_##typename* self, type value, ctd_error* error)                         \
    {                                                                                                                  \
        if (self == NULL)                                                                                              \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was NULL";                                                                   \
                                                                                                                       \
            return;                                                                                                    \
        }                                                                                                              \
                                                                                                                       \
        self->data[self->length - 1] = value;                                                                          \
        self->length++;                                                                                                \
        ctd_stack_##typename##_maybe_expand(self, error);                                                              \
    }                                                                                                                  \
                                                                                                                       \
    type ctd_stack_##typename##_pop(ctd_stack_##typename* self, ctd_error* error)                                      \
    {                                                                                                                  \
        if (self == NULL)                                                                                              \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was NULL";                                                                   \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
        if (self->length == 0)                                                                                         \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was empty";                                                                  \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        type popped_element = self->data[self->length - 1];                                                            \
        ctd_stack_##typename##_maybe_contract(self, error);                                                            \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            int empty = {0};                                                                                           \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return popped_element;                                                                                         \
    }                                                                                                                  \
    type ctd_stack_##typename##_peek(ctd_stack_##typename* self, ctd_error* error)                                     \
    {                                                                                                                  \
        if (self == NULL)                                                                                              \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was NULL";                                                                   \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
        if (self->length == 0)                                                                                         \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was empty";                                                                  \
                                                                                                                       \
            type empty = {0};                                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return self->data[self->length - 1];                                                                           \
    }                                                                                                                  \
    void ctd_stack_##typename##_resize(ctd_stack_##typename* self, ptrdiff_t new_capacity, ctd_error* error)           \
    {                                                                                                                  \
        type* new_data = realloc(self->data, new_capacity);                                                            \
        if (new_data == NULL)                                                                                          \
        {                                                                                                              \
            error->error_type = ALLOCATION_FAIL;                                                                       \
            error->error_message = "Realloc Failed.";                                                                  \
            return;                                                                                                    \
        }                                                                                                              \
        self->capacity = new_capacity;                                                                                 \
        self->data = new_data;                                                                                         \
    }                                                                                                                  \
    void ctd_stack_##typename##_maybe_expand(ctd_stack_##typename* self, ctd_error* error)                             \
    {                                                                                                                  \
        if (self->length == self->capacity)                                                                            \
        {                                                                                                              \
            ctd_stack_##typename##_resize(self, self->capacity * 2 + 1, error);                                        \
        }                                                                                                              \
    }                                                                                                                  \
    void ctd_stack_##typename##_maybe_contract(ctd_stack_##typename* self, ctd_error* error)                           \
    {                                                                                                                  \
        if (self->length < self->capacity / 4)                                                                         \
        {                                                                                                              \
            ctd_stack_##typename##_resize(self, self->capacity / 2, error);                                            \
        }                                                                                                              \
    }                                                                                                                  \
    ctd_stack_##typename##_iterator ctd_stack_##                                                                       \
        typename##_iterator_begin(ctd_stack_##typename* ctd_stack, ctd_error* error)                                   \
    {                                                                                                                  \
        if (ctd_stack == NULL)                                                                                         \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was NULL";                                                                   \
                                                                                                                       \
            ctd_stack_##typename##_iterator empty = {0};                                                               \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return (ctd_stack_##typename##_iterator){.ptr = ctd_stack->data};                                              \
    }                                                                                                                  \
    ctd_stack_##typename##_iterator ctd_stack_##typename##_iterator_end(ctd_stack_##typename* ctd_stack, ctd_error* error) \
    {                                                                                                                  \
        if (ctd_stack == NULL)                                                                                         \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was NULL";                                                                   \
                                                                                                                       \
            ctd_stack_##typename##_iterator empty = {0};                                                               \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return (ctd_stack_##typename##_iterator){.ptr = ctd_stack->data + ctd_stack->length};                          \
    }                                                                                                                  \
    void ctd_stack_##typename##_iterator_increment(ctd_stack_##typename##_iterator* iterator, ctd_error* error)        \
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
#endif
