#ifndef CTD_STRING_H
#define CTD_STRING_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <allocator.h>
#include <misc_utils.h>
#include <error_message.h>
#include <option.h>

#ifndef DEFAULT_OPTION_TYPES_DECL
#define DEFAULT_OPTION_TYPES_DECL
DEFAULT_OPTION_TYPES(OPTION_TYPE_DECL)
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
    allocator_t* allocator;
} ctd_string_builder;

ctd_string ctd_string_create(ptrdiff_t length, allocator_t allocator, Error* error);
#define ctd_string_create_from_literal(literal) \
    (ctd_string) { .data = literal, .length = countof(literal) - 1 }
ctd_string ctd_string_span(char *beginning, char *end);
bool ctd_string_equals(ctd_string a, ctd_string b);
ptrdiff_t ctd_string_compare(ctd_string a, ctd_string b);
option(ptrdiff_t) ctd_string_find(ctd_string str, ctd_string substring, ptrdiff_t start, Error* error);
option(ptrdiff_t) ctd_string_reverse_find(ctd_string str, ctd_string substring, ptrdiff_t end, Error* error);
uint64_t ctd_string_hash(ctd_string str);
ctd_string ctd_string_remove_whitespace(ctd_string str, allocator_t allocator, Error* error);
ctd_string ctd_string_copy(ctd_string str, allocator_t allocator, Error* error);
char* ctd_string_to_c_string(ctd_string str, allocator_t allocator, Error* error);

ctd_string_builder ctd_string_builder_create(ptrdiff_t capacity, allocator_t* allocator, Error* error);
void ctd_string_builder_push_back(ctd_string_builder* self, char c, Error* error);
void ctd_string_builder_pop_back(ctd_string_builder* self, Error* error);
void ctd_string_builder_append(ctd_string_builder *self, ctd_string str, Error* error);
void ctd_string_builder_insert(ctd_string_builder *self, ctd_string str, ptrdiff_t index, Error* error);
void ctd_string_builder_remove(ctd_string_builder* self, ptrdiff_t index, ptrdiff_t length, Error* error);
void ctd_string_builder_replace_at(ctd_string_builder* self, ctd_string replacement, ptrdiff_t index, ptrdiff_t length, Error* error);
option(ptrdiff_t) ctd_string_builder_find(ctd_string_builder* self, ctd_string substring, ptrdiff_t start, Error* error);
option(ptrdiff_t) ctd_string_builder_reverse_find(ctd_string_builder* self, ctd_string substring, ptrdiff_t end, Error* error);
bool ctd_string_builder_contains(ctd_string_builder* self, ctd_string substring, Error* error);
void ctd_string_builder_replace(ctd_string_builder* self, ctd_string substring, ctd_string replacement, ptrdiff_t start, Error* error);
void ctd_string_builder_replace_all(ctd_string_builder* self, ctd_string substring, ctd_string replacement, ptrdiff_t start, Error* error);
void ctd_string_builder_reverse(ctd_string_builder* self);
void ctd_string_builder_clear(ctd_string_builder* self);
ctd_string ctd_string_builder_to_substring(ctd_string_builder *self, ptrdiff_t index, ptrdiff_t length,
                                           allocator_t allocator, Error* error);
ctd_string ctd_string_builder_to_string(ctd_string_builder *self, allocator_t allocator, Error* error);

#endif