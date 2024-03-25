#ifndef GENERIC_STACK_H
#define GENERIC_STACK_H
#include <error_message.h>
#include <misc_utils.h>
#include <stdlib.h>

#define FORMAT_STACK(type, typename, function_name) , phs_stack_##typename* : phs_stack_##typename##_##function_name

#define phs_stack(typename) phs_stack_##typename

#define phs_stack_create(typename, capacity, error_ptr) phs_stack_##typename##_create((capacity), (error_ptr))

#define phs_stack_initialize(typename, variable_name, capacity, error_ptr)                                             \
    phs_stack_##typename variable_name = phs_stack_##typename##_create((capacity), (error_ptr))

#define phs_stack_destroy(stack_ptr)                                                                                   \
_Generic((stack_ptr), REMOVE_LEADING_COMMA(CTD_STACK_TYPES(FORMAT_STACK, destroy))(stack_ptr)

#define phs_stack_push(stack_ptr, value, error_ptr)                                                                    \
_Generic((stack_ptr), REMOVE_LEADING_COMMA(CTD_STACK_TYPES(FORMAT_STACK, push))(stack_ptr, value, error_ptr)

#define phs_stack_pop(stack_ptr, error_ptr)                                                                            \
_Generic((stack_ptr), REMOVE_LEADING_COMMA(CTD_STACK_TYPES(FORMAT_STACK, pop))(stack_ptr, error_ptr)

#define phs_stack_peek(stack_ptr, error_ptr)                                                                           \
_Generic((stack_ptr), REMOVE_LEADING_COMMA(CTD_STACK_TYPES(FORMAT_STACK, peek))(stack_ptr, error_ptr)

#define GENERIC_STACK_TYPE_DECL(type, typename, ...)                                                                   \
    typedef struct phs_stack_##typename                                                                                \
    {                                                                                                                  \
        type* data;                                                                                                    \
        size_t length;                                                                                                 \
        size_t capacity;                                                                                               \
    }                                                                                                                  \
    phs_stack_##typename;                                                                                              \
    typedef struct phs_stack_##typename##_iterator                                                                     \
    {                                                                                                                  \
        type* ptr;                                                                                                     \
    }                                                                                                                  \
    phs_stack_##typename##_iterator;

#define GENERIC_STACK_FUNCTIONS_DECL(type, typename, ...)                                                              \
    phs_stack_##typename phs_stack_##typename##_create(size_t capacity, Error* error);                                 \
    void phs_stack_##typename##_destroy(phs_stack_##typename* self);                                                   \
    void phs_stack_##typename##_push(phs_stack_##typename* self, type value, Error* error);                            \
    type phs_stack_##typename##_pop(phs_stack_##typename* self, Error* error);                                         \
    type phs_stack_##typename##_peek(phs_stack_##typename* self, Error* error);                                        \
    phs_stack_##typename##_iterator phs_stack_##                                                                       \
        typename##_iterator_begin(phs_stack_##typename* phs_stack, Error* error);                                      \
    phs_stack_##typename##_iterator phs_stack_##                                                                       \
        typename##_iterator_end(phs_stack_##typename* phs_stack, Error* error);                                        \
    void phs_stack_##typename##_iterator_increment(phs_stack_##typename##_iterator* iterator, Error* error);

#define GENERIC_STACK_IMPL(type, typename, ...)                                                                        \
    void phs_stack_##typename##_resize(phs_stack_##typename* self, size_t new_capacity, Error* error);                 \
    void phs_stack_##typename##_maybe_expand(phs_stack_##typename* self, Error* error);                                \
    void phs_stack_##typename##_maybe_contract(phs_stack_##typename* self, Error* error);                              \
    phs_stack_##typename phs_stack_##typename##_create(size_t capacity, Error* error)                                  \
    {                                                                                                                  \
        if (capacity == 0)                                                                                             \
        {                                                                                                              \
            capacity = 1;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        phs_stack_##typename stack = {0};                                                                              \
        stack.length = 0;                                                                                              \
        stack.capacity = capacity;                                                                                     \
        stack.data = malloc(sizeof(type) * stack.capacity);                                                            \
        if (stack.data == NULL)                                                                                        \
        {                                                                                                              \
            error->error_type = ALLOCATION_FAIL;                                                                       \
            error->error_message = "Malloc Failed.";                                                                   \
                                                                                                                       \
            phs_stack_##typename empty = {0};                                                                          \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return stack;                                                                                                  \
    }                                                                                                                  \
                                                                                                                       \
    void phs_stack_##typename##_destroy(phs_stack_##typename* self)                                                    \
    {                                                                                                                  \
        if (self->data != NULL)                                                                                        \
        {                                                                                                              \
            free(self->data);                                                                                          \
        }                                                                                                              \
        phs_stack_##typename empty = {0};                                                                              \
        *self = empty;                                                                                                 \
    }                                                                                                                  \
                                                                                                                       \
    void phs_stack_##typename##_push(phs_stack_##typename* self, type value, Error* error)                             \
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
        phs_stack_##typename##_maybe_expand(self, error);                                                              \
    }                                                                                                                  \
                                                                                                                       \
    type phs_stack_##typename##_pop(phs_stack_##typename* self, Error* error)                                          \
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
        phs_stack_##typename##_maybe_contract(self, error);                                                            \
        if (error->error_type != NO_ERROR)                                                                             \
        {                                                                                                              \
            int empty = {0};                                                                                           \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return popped_element;                                                                                         \
    }                                                                                                                  \
    type phs_stack_##typename##_peek(phs_stack_##typename* self, Error* error)                                         \
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
    void phs_stack_##typename##_resize(phs_stack_##typename* self, size_t new_capacity, Error* error)                  \
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
    void phs_stack_##typename##_maybe_expand(phs_stack_##typename* self, Error* error)                                 \
    {                                                                                                                  \
        if (self->length == self->capacity)                                                                            \
        {                                                                                                              \
            phs_stack_##typename##_resize(self, self->capacity * 2 + 1, error);                                        \
        }                                                                                                              \
    }                                                                                                                  \
    void phs_stack_##typename##_maybe_contract(phs_stack_##typename* self, Error* error)                               \
    {                                                                                                                  \
        if (self->length < self->capacity / 4)                                                                         \
        {                                                                                                              \
            phs_stack_##typename##_resize(self, self->capacity / 2, error);                                            \
        }                                                                                                              \
    }                                                                                                                  \
    phs_stack_##typename##_iterator phs_stack_##                                                                       \
        typename##_iterator_begin(phs_stack_##typename* phs_stack, Error* error)                                       \
    {                                                                                                                  \
        if (phs_stack == NULL)                                                                                         \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was NULL";                                                                   \
                                                                                                                       \
            phs_stack_##typename##_iterator empty = {0};                                                               \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return (phs_stack_##typename##_iterator){.ptr = phs_stack->data};                                              \
    }                                                                                                                  \
    phs_stack_##typename##_iterator phs_stack_##typename##_iterator_end(phs_stack_##typename* phs_stack, Error* error) \
    {                                                                                                                  \
        if (phs_stack == NULL)                                                                                         \
        {                                                                                                              \
            error->error_type = INVALID_ARGUMENT;                                                                      \
            error->error_message = "Stack was NULL";                                                                   \
                                                                                                                       \
            phs_stack_##typename##_iterator empty = {0};                                                               \
            return empty;                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        return (phs_stack_##typename##_iterator){.ptr = phs_stack->data + phs_stack->length};                          \
    }                                                                                                                  \
    void phs_stack_##typename##_iterator_increment(phs_stack_##typename##_iterator* iterator, Error* error)            \
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
