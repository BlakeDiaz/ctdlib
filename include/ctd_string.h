#ifndef DYN_STRING_H
#define DYN_STRING_H

#include <error_message.h>
#include <option.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef DEFAULT_OPTION_TYPES_DECL
#define DEFAULT_OPTION_TYPES_DECL
DEFAULT_OPTION_TYPES(OPTION_TYPE_DECL)
#endif

/**
 * Length-based string.
 * No null-terminator characters.
 */
typedef struct CTD_String
{
    char* data;
    ptrdiff_t length;
    ptrdiff_t capacity;
} CTD_String;

/**
 * Doesn't need to be malloced or freed.
 * Note - unsafe to access if parent CTD_String is destroyed.
 * Be careful using this after modifying parent CTD_String in any way - parent CTD_String may be reallocated, and
 * CTD_String_View will become invalid as a result.
 */
typedef struct CTD_String_View
{
    char* data;
    ptrdiff_t length;
} CTD_String_View;

typedef struct CTD_Strings
{
    CTD_String* data;
    ptrdiff_t length;
} CTD_Strings;

typedef struct CTD_String_Views
{
    CTD_String_View* data;
    ptrdiff_t length;
} CTD_String_Views;

typedef struct Find_Indices
{
    ptrdiff_t* data;
    ptrdiff_t length;
} Find_Indices;

#define ctd_string_create_from_macro(source, error)                                                                    \
    _Generic((source),                                                                                                 \
        CTD_String: ctd_string_copy,                                                                                   \
        CTD_String_View,                                                                                               \
        ctd_string_create_from_view,                                                                                   \
        const char*: ctd_string_create_from_c_string)(source, error)
CTD_String ctd_string_create(const ptrdiff_t capacity, Error* error);
CTD_String ctd_string_create_from_view(const CTD_String_View view, Error* error);
CTD_String ctd_string_create_from_c_string(const char* c_string, Error* error);
CTD_String ctd_string_copy(const CTD_String str, Error* error);
CTD_String ctd_string_copy_view(const CTD_String_View str, Error* error);
void ctd_string_destroy(CTD_String* str);

CTD_String_View ctd_string_view_create(const CTD_String str, const ptrdiff_t start_index, const ptrdiff_t end_index,
                                       Error* error);
CTD_String_View ctd_string_view_create_from_c_string(char* c_string, const ptrdiff_t start_index,
                                                     const ptrdiff_t end_index, Error* error);
CTD_String_View ctd_string_view_create_from_char_ptr(char* data, const ptrdiff_t length);

CTD_String ctd_string_concat(const CTD_String a, const CTD_String b, Error* error);
CTD_String ctd_string_concat_view(const CTD_String_View a, const CTD_String_View b, Error* error);
void ctd_string_append_inner(CTD_String* str, CTD_String_View to_append, Error* error);
/**
 * Appends to a CTD_String
 *
 * @param str CTD_String to be appended to.
 * @param to_append Text appended to str. Can be a CTD_String, CTD_String_View, or a null-terminated c string.
 * @param error Pointer to Error struct.
 */
#define ctd_string_append(str, to_append, error) ctd_string_append_inner(str, to_view(to_append), error)
void ctd_string_insert_inner(CTD_String* str, const CTD_String_View to_insert, const ptrdiff_t insert_index,
                             Error* error);
/**
 * Inserts a string into another string.
 *
 * @param str String to be inserted into. Can be a CTD_String, CTD_String_View, or a null-terminated c string.
 * @param to_insert Text inserted into str. Can be a CTD_String, CTD_String_View, or a null-terminated c string.
 * @param index Index the text will be inserted at. For example, if str is "Jon", to_insert is "h", and the index is 2,
 * the resulting string is "John".
 * @param error Pointer to Error struct.
 */
#define ctd_string_insert(str, to_insert, insert_index, error)                                                         \
    ctd_string_insert_inner(str, to_view(to_insert), insert_index, error)
void ctd_string_clear(CTD_String* str, Error* error);

bool ctd_string_is_empty_inner(const CTD_String_View str);
#define ctd_string_is_empty(str) ctd_string_is_empty_inner(to_view(str))
// bool ctd_string_starts_with(CTD_String_View str, CTD_String_View);
bool ctd_string_equals_inner(const CTD_String_View a, const CTD_String_View b);
#define ctd_string_equals(a, b) ctd_string_equals_inner(to_view(a), to_view(b))
bool ctd_string_contains(const CTD_String str, const CTD_String contained_str);
option(ptrdiff_t) ctd_string_find_inner(const CTD_String_View str, const CTD_String_View substring,
                                        ptrdiff_t starting_index, Error* error);
#define ctd_string_find(str, substring, starting_index, error)                                                         \
    ctd_string_find_inner(to_view(str), to_view(substring), starting_index, error)

/**
 * Replaces a section of characters with a string.
 *
 * @param str String to be modified.
 * @param starting_index Starting index of replacement. Inclusive.
 * @param count Number of characters to be replaced.
 * @param replacement String to be inserted.
 * @param error Pointer to Error struct.
 */
void ctd_string_replace_inner(CTD_String* str, ptrdiff_t starting_index, ptrdiff_t count,
                              const CTD_String_View replacement, Error* error);
#define ctd_string_replace(str, starting_index, count, replacement, error)                                             \
    ctd_string_replace_inner(str, starting_index, count, to_view(replacement), error)

CTD_Strings ctd_string_split_inner(const CTD_String_View str, const CTD_String_View delimiter, Error* error);
CTD_String_Views ctd_string_split_views_inner(const CTD_String_View str, const CTD_String_View delimiter, Error* error);

CTD_String ctd_string_remove_whitespace_inner(CTD_String_View str, Error* error);
#define ctd_string_remove_whitespace(str, error) ctd_string_remove_whitespace_inner(to_view(str), error);

CTD_String_View ctd_string_view_create_from_full_string_(const CTD_String str);
CTD_String_View ctd_string_view_create_from_view_(const CTD_String_View view);
CTD_String_View ctd_string_view_create_from_full_c_string_(char* c_str);

char* ctd_string_to_c_string_inner(CTD_String_View str, Error* error);
#define ctd_string_to_c_string(str, error_ptr) ctd_string_to_c_string_inner(to_view(str, error_ptr))
/**
 * Convenience macro to convert CTD_String into CTD_String_View containing full contents of string
 *
 * @param str CTD_String to be converted into view
 * @return CTD_String_View
 */
#define to_view_get_macro(_1, _2, NAME, ...) NAME
#define to_view_1_arg(str)                                                                                             \
    _Generic((str),                                                                                                    \
        CTD_String: ctd_string_view_create_from_full_string_,                                                          \
        CTD_String_View: ctd_string_view_create_from_view_,                                                            \
        char*: ctd_string_view_create_from_full_c_string_)(str)
#define to_view_2_arg(str, len)                                                                                        \
    _Generic((str), const char*: ctd_string_view_create_from_char_ptr, char*: ctd_string_view_create_from_char_ptr)(   \
        str, len)
#define to_view(...) to_view_get_macro(__VA_ARGS__, to_view_2_arg, to_view_1_arg)(__VA_ARGS__)

#define slice(start, stop, step)

/**
 * Splits a string based on a delimiter into individual CTD_Strings.
 *
 * @param str String to be split. Can be a CTD_String, a CTD_String_View, or a null-terminated c string.
 * @param delimiter String used to split str. Can be a CTD_String, a CTD_String_View, or a null-terminated c string.
 * @param error Pointer to Error struct.
 * @return Struct containing a heap allocated array of CTD_Strings split from the original string.
 */
#define ctd_string_split(str, delimiter, error) ctd_string_split_inner(to_view(str), to_view(delimiter), error)

/**
 * Splits a string based on a delimiter into individual CTD_String_Views.
 *
 * @param str String to be split. Can be a CTD_String, a CTD_String_View, or a null-terminated c string.
 * @param delimiter String used to split str. Can be a CTD_String, a CTD_String_View, or a null-terminated c string.
 * @param error Pointer to Error struct.
 * @return Struct containing a heap allocated array of CTD_String_Views split from the original string.
 */
#define ctd_string_split_views(str, delimiter, error)                                                                  \
    ctd_string_split_views_inner(to_view(str), to_view(delimiter), error)

#endif // DYN_STRING_H
