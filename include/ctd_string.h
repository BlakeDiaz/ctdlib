#ifndef CTD_STRING_H
#define CTD_STRING_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <ctd_allocator.h>
#include <ctd_define.h>
#include <ctd_error_message.h>
#include <ctd_option.h>

#ifndef DEFAULT_OPTION_TYPES_DECL
#define DEFAULT_OPTION_TYPES_DECL
CTD_DEFAULT_OPTION_TYPES(CTD_OPTION_TYPE_DECL)
#endif

typedef struct ctd_string
{
    ptrdiff_t length;
    char *data;
} ctd_string;

typedef struct ctd_string_builder
{
    ptrdiff_t length;
    ptrdiff_t capacity;
    char *data;
    ctd_allocator* allocator;
} ctd_string_builder;

ctd_string ctd_string_create(ptrdiff_t length, ctd_allocator allocator, ctd_error* error);
#define ctd_string_create_from_literal(literal) \
    (ctd_string) { .data = literal, .length = countof(literal) - 1 }
ctd_string ctd_string_span(char *beginning, char *end);
bool ctd_string_equals(ctd_string a, ctd_string b);
ptrdiff_t ctd_string_compare(ctd_string a, ctd_string b);
ctd_option(ptrdiff_t) ctd_string_find(ctd_string str, ctd_string substring, ptrdiff_t start, ctd_error* error);
ctd_option(ptrdiff_t) ctd_string_reverse_find(ctd_string str, ctd_string substring, ptrdiff_t end, ctd_error* error);
uint64_t ctd_string_hash(ctd_string str);
ctd_string ctd_string_remove_whitespace(ctd_string str, ctd_allocator allocator, ctd_error* error);
ctd_string ctd_string_copy(ctd_string str, ctd_allocator allocator, ctd_error* error);
char* ctd_string_to_c_string(ctd_string str, ctd_allocator allocator, ctd_error* error);
void ctd_string_destroy(ctd_string* self, ctd_allocator allocator);

ctd_string_builder ctd_string_builder_create(ptrdiff_t capacity, ctd_allocator* allocator, ctd_error* error);
void ctd_string_builder_push_back(ctd_string_builder* self, char c, ctd_error* error);
void ctd_string_builder_pop_back(ctd_string_builder* self, ctd_error* error);
void ctd_string_builder_append(ctd_string_builder *self, ctd_string str, ctd_error* error);
void ctd_string_builder_insert(ctd_string_builder *self, ctd_string str, ptrdiff_t index, ctd_error* error);
void ctd_string_builder_remove(ctd_string_builder* self, ptrdiff_t index, ptrdiff_t length, ctd_error* error);
void ctd_string_builder_replace_at(ctd_string_builder* self, ctd_string replacement, ptrdiff_t index, ptrdiff_t length, ctd_error* error);
ctd_option(ptrdiff_t) ctd_string_builder_find(ctd_string_builder* self, ctd_string substring, ptrdiff_t start, ctd_error* error);
ctd_option(ptrdiff_t) ctd_string_builder_reverse_find(ctd_string_builder* self, ctd_string substring, ptrdiff_t end, ctd_error* error);
bool ctd_string_builder_contains(ctd_string_builder* self, ctd_string substring, ctd_error* error);
void ctd_string_builder_replace(ctd_string_builder* self, ctd_string substring, ctd_string replacement, ptrdiff_t start, ctd_error* error);
void ctd_string_builder_replace_all(ctd_string_builder* self, ctd_string substring, ctd_string replacement, ptrdiff_t start, ctd_error* error);
void ctd_string_builder_reverse(ctd_string_builder* self);
void ctd_string_builder_clear(ctd_string_builder* self);
ctd_string ctd_string_builder_to_substring(ctd_string_builder *self, ptrdiff_t index, ptrdiff_t length,
                                           ctd_allocator allocator, ctd_error* error);
ctd_string ctd_string_builder_to_string(ctd_string_builder *self, ctd_allocator allocator, ctd_error* error);
ctd_string ctd_string_builder_to_span(ctd_string_builder *self, ptrdiff_t start, ptrdiff_t end, ctd_error* error);
void ctd_string_builder_destroy(ctd_string_builder *self);

#endif