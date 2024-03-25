#include <error_message.h>
#include <option.h>
#include <stdlib.h>
#include <string.h>

#include <internal_dyn_array.h>
#include <misc_utils.h>
#include <ctd_string.h>

#include <generic_dynamic_array.h>

#define CTD_DYNAMIC_ARRAY_TYPES(X)\
X(CTD_String, string, __VA_ARGS__)\
X(CTD_String_View, string_view, __VA_ARGS__)

CTD_DYNAMIC_ARRAY_TYPES(DYNAMIC_ARRAY_TYPE_DECL)
CTD_DYNAMIC_ARRAY_TYPES(DYNAMIC_ARRAY_FUNCTIONS_DECL)
CTD_DYNAMIC_ARRAY_TYPES(DYNAMIC_ARRAY_IMPL)

void ctd_string_resize(CTD_String* str, const size_t new_capacity, Error* error);
void ctd_string_maybe_expand(CTD_String* str, const size_t expand_by, Error* error);
void ctd_string_maybe_contract(CTD_String* str, Error* error);

/**
 * Creates an empty CTD_String.
 * Data is allocated on the heap.
 *
 * @param capacity Initial capacity of CTD_String.
 * @param error Pointer to Error struct.
 * @return CTD_String.
 */
CTD_String ctd_string_create(const size_t capacity, Error* error)
{
    CTD_String string = {0};
    string.capacity = capacity;
    string.data = malloc(capacity * sizeof(char));
    if (string.data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Malloc Failed.";
        
        return (CTD_String) {0};
    }

    return string;
}

/**
 * Creates a CTD_String from a CTD_String_View.
 * Data is allocated on the heap.
 *
 * @param view CTD_String_View that data is copied from.
 * @param error Pointer to Error struct.
 * @return CTD_String.
 */
CTD_String ctd_string_create_from_view(const CTD_String_View view, Error* error)
{
    CTD_String string = {.data = malloc(view.length * sizeof(char)), .length = view.length, .capacity = view.length};
    if (string.data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Malloc Failed.";

        return (CTD_String) {0};
    }

    memcpy(string.data, view.data, view.length * sizeof(char));

    return string;
}

/**
 * Creates a CTD_String from a null terminated c string.
 * Data is allocated on the heap.
 *
 * @param c_string Null terminated c string that data is copied from.
 * @param error Pointer to Error struct.
 * @return CTD_String.
 */
CTD_String ctd_string_create_from_c_string(const char* c_string, Error* error)
{
    if (c_string == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "C_string was NULL";

        CTD_String empty = {0};
        return empty;
    }

    size_t new_string_length = strlen(c_string);
    CTD_String new_string = {.data = malloc(new_string_length * sizeof(char)), .length = new_string_length, .capacity = new_string_length};
    if (new_string.data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Malloc Failed.";

        return (CTD_String) {0};
    }

    memcpy(new_string.data, c_string, new_string_length);

    return new_string;
}

CTD_String ctd_string_create_from_char_ptr(const char* data, size_t length, Error* error)
{
    if (data == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "C_string was NULL";

        CTD_String empty = {0};
        return empty;
    }

    CTD_String new_string = {.data = malloc(length * sizeof(char)), .length = length, .capacity = length};
    if (new_string.data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Malloc Failed.";

        return (CTD_String) {0};
    }

    memcpy(new_string.data, data, length);

    return new_string;
}

/**
 * Copies a CTD_String.
 * Data is allocated on the heap.
 *
 * @param str CTD_String that data is copied from.
 * @param error Pointer to Error struct.
 * @return CTD_String.
 */
CTD_String ctd_string_copy(const CTD_String str, Error* error)
{
    CTD_String new_string = ctd_string_create(str.length, error);
    if (error->error_type != NO_ERROR)
    {
        return (CTD_String) {0};
    }

    memcpy(new_string.data, str.data, str.length * sizeof(char));

    return new_string;
}

/**
 * Copies a CTD_String from a CTD_String_View.
 * Data is allocated on the heap.
 *
 * @param str CTD_String_View that data is copied from.
 * @param error Pointer to Error struct.
 * @return CTD_String.
 */
CTD_String ctd_string_copy_view(CTD_String_View str, Error* error)
{
    CTD_String new_string = ctd_string_create(str.length, error);
    if (error->error_type != NO_ERROR)
    {
        return (CTD_String) {0};
    }

    memcpy(new_string.data, str.data, str.length * sizeof(char));

    return new_string;
}

/**
 * Destroys a CTD_String.
 * Data is is freed and string is zeroed out, so it is equivalent to (CTD_String) {0}.
 *
 * @param str CTD_String to be destroyed.
 */
void ctd_string_destroy(CTD_String* str)
{
    if (str->data != NULL)
    {
        free(str->data);
    }
    CTD_String empty = {0};
    *str = empty;
}

/**
 * Creates a CTD_String_View from a section of a CTD_String.
 *
 * @param str CTD_String that view is taken from.
 * @param start_index Starting index of view. Inclusive.
 * @param end_index Ending index of view. Exclusive.
 * @param error Pointer to Error struct.
 * @return CTD_String_View.
 */
CTD_String_View ctd_string_view_create(const CTD_String str, const size_t start_index, const size_t end_index, Error* error)
{
    if (start_index > end_index)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Invalid Index: start index > end index";

        CTD_String_View empty = {0};
        return empty;
    }

    if (end_index > str.length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Invalid Index: end index exceeds length of string";

        CTD_String_View empty = {0};
        return empty;
    }

    CTD_String_View view = {.data = str.data + start_index, .length = end_index - start_index};

    return view;
}

/**
 * Creates a CTD_String_View from a section of a null terminated c string.
 *
 * @param str Null terminated c string that view is taken from.
 * @param start_index Starting index of view. Inclusive.
 * @param end_index Ending index of view. Exclusive.
 * @param error Pointer to Error struct.
 * @return CTD_String_View.
 */
CTD_String_View ctd_string_view_create_from_c_string(char* c_string, const size_t start_index,
                                                     const size_t end_index, Error* error)
{
    if (start_index > end_index)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Invalid Index: start index > end index";

        CTD_String_View empty = {0};
        return empty;
    }
    size_t length = strlen(c_string);

    if (end_index > length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Invalid Index: end index exceeds length of string";

        CTD_String_View empty = {0};
        return empty;
    }

    CTD_String_View view = {.data = c_string + start_index, .length = end_index - start_index};

    return view;
}

inline CTD_String_View ctd_string_view_create_from_char_ptr(char* data, const size_t length)
{
    return (CTD_String_View) {.data = data, .length = length};
}

/**
 * Concatenates CTD_Strings into a new CTD_String.
 *
 * @param a First string to be concatenated.
 * @param b Second string to be concatenated.
 * @param error Pointer to Error struct.
 * @return CTD_String.
 */
CTD_String ctd_string_concat(const CTD_String a, const CTD_String b, Error* error)
{
    size_t new_string_length = a.length + b.length;
    CTD_String new_string = {0};
    new_string.length = new_string_length;
    new_string.capacity = new_string_length;
    new_string.data = malloc(new_string_length);
    if (new_string.data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Malloc Failed.";

        CTD_String empty = {0};
        return empty;
    }

    memcpy(new_string.data, a.data, a.length);
    memcpy(new_string.data + a.length, b.data, b.length);

    return new_string;
}

/**
 * Concatenates two CTD_String_Views into a new CTD_String.
 *
 * @param a First view to be concatenated.
 * @param b Second view to be concatenated.
 * @param error Pointer to Error struct.
 * @return CTD_String.
 */
CTD_String ctd_string_concat_view(const CTD_String_View a, const CTD_String_View b, Error* error)
{
    size_t new_string_length = a.length + b.length;
    CTD_String new_string = {0};
    new_string.length = new_string_length;
    new_string.capacity = new_string_length;
    new_string.data = malloc(new_string_length);
    if (new_string.data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Malloc Failed.";

        CTD_String empty = {0};
        return empty;
    }

    memcpy(new_string.data, a.data, a.length);
    memcpy(new_string.data + a.length, b.data, b.length);

    return new_string;
}

void ctd_string_append_inner(CTD_String* str, CTD_String_View to_append, Error* error)
{
    if (str == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "CTD_String to append to was null";
        return;
    }

    ctd_string_maybe_expand(str, to_append.length, error);
    if (error->error_type != NO_ERROR)
    {
        return;
    }

    memcpy(str->data + str->length, to_append.data, to_append.length * sizeof(char));

    str->length += to_append.length;
}

void ctd_string_insert_inner(CTD_String* str, CTD_String_View to_insert, const size_t index, Error* error)
{
    if (str == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "CTD_String to append to was null";
        return;
    }
    if (index > str->length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Invalid insert index";
        return;
    }
    if (index == str->length)
    {
        ctd_string_append_inner(str, to_insert, error);
        if (error->error_type != NO_ERROR)
        {
            return;
        }
    }

    ctd_string_maybe_expand(str, to_insert.length, error);
    if (error->error_type != NO_ERROR)
    {
        return;
    }

    // Move characters at and after index to the right to make room for the inserted text
    // TODO replace w/ memmove
    for (size_t i = str->length - 1; i >= index; i--)
    {
        str->data[i + to_insert.length] = str->data[i];
    }

    memcpy(str->data + (index * sizeof(char)), to_insert.data, to_insert.length * sizeof(char));

    str->length += to_insert.length;
}

/**
 * Clears the contents of a CTD_String.
 *
 * @param str CTD_String to be cleared.
 * @param error Pointer to Error struct.
 */
void ctd_string_clear(CTD_String* str, Error* error)
{
    if (str == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "CTD_String to clear to was null";

        return;
    }

    memset(str->data, 0, str->length * sizeof(char));
}

inline bool ctd_string_is_empty_inner(const CTD_String_View str)
{
    return str.length == 0;
}

bool ctd_string_equals_inner(const CTD_String_View a, const CTD_String_View b)
{
    if (a.length != b.length)
    {
        return false;
    }

    for (int i = 0; i < a.length; i++)
    {
        if (a.data[i] != b.data[i])
        {
            return false;
        }
    }

    return true;
}

/**
 * Checks if a substring is contained within a CTD_String_View.
 * Returns false if either string is empty.
 *
 * @param str CTD_String_View checked to see if substring resides in it
 * @param substring CTD_String possibly contained inside str.
 * @return true if the string is contained, false otherwise.
 *
 */
bool ctd_string_contains_inner(const CTD_String_View str, const CTD_String_View substring)
{
    if (ctd_string_is_empty(str) || ctd_string_is_empty(substring))
    {
        return false;
    }
    if (substring.length > str.length)
    {
        return false;
    }

    for (size_t i = 0; i < str.length; i++)
    {
        // If this is true, there isn't enough room inside str to contain the substring
        if (str.length - i < substring.length)
        {
            return false;
        }
        // If this is true, then the substring can't start at this character, and we move onto the next one
        if (str.data[i] != substring.data[0])
        {
            continue;
        }

        CTD_String_View str_view = {.data = str.data + i, .length = substring.length};
        if (ctd_string_equals(str_view, substring))
        {
            return true;
        }
    }

    return false;
}

option(size_t) ctd_string_find_inner(const CTD_String_View str, const CTD_String_View substring, size_t starting_index, Error* error)
{
    if (starting_index >= str.length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Starting index was greater than or equal to string's length.";

        return NONE(size_t);
    }

    if (str.length - starting_index < substring.length)
    {
        return NONE(size_t);
    }
    if (ctd_string_is_empty(str) || substring.length == 0)
    {
        return NONE(size_t);
    }

    for (size_t i = starting_index; i < str.length; i++)
    {
        // If this is true, there isn't enough room inside str to contain the substring
        if (str.length - i < substring.length)
        {
            return NONE(size_t);
        }
        // If this is true, then the substring can't start at this character, and we move onto the next one
        if (str.data[i] != substring.data[0])
        {
            continue;
        }

        CTD_String_View str_view = {.data = str.data + i, .length = substring.length};
        if (ctd_string_equals(str_view, substring))
        {
            return SOME(size_t, i);
        }
    }

    return NONE(size_t);
}

Find_Indices ctd_string_find_all_inner(const CTD_String_View str, const CTD_String_View delimiter, size_t starting_index, Error* error)
{
    ctd_internal_dynamic_array(size_t) indices = {.data = malloc(sizeof(size_t)), .length = 0, .capacity = 1};
    option(size_t) index;

    index = ctd_string_find_inner(str, delimiter, starting_index, error);
    while (IS_SOME(index))
    {
        ctd_internal_dynamic_array_append(indices, size_t, index.value, error);
        if (error->error_type != NO_ERROR)
        {
            free(indices.data);
            return (Find_Indices) {0};
        }
        starting_index += delimiter.length;
        index = ctd_string_find_inner(str, delimiter, starting_index, error);
    }

    if (indices.length == 0)
    {
        free(indices.data);
        return (Find_Indices) {0};
    }

    return (Find_Indices) {0};
}

void ctd_string_replace_inner(CTD_String* str, size_t start_position, size_t count, const CTD_String_View replacement, Error* error)
{
    if (start_position >= str->length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Invalid start position";

        return;
    }
    if (start_position + count > str->length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Invalid start position and count.";

        return;
    }

    ctd_string_maybe_expand(str, replacement.length - count, error);
    if (error->error_type != NO_ERROR)
    {
        return;
    }
    size_t bytes_to_move = (str->length - start_position - count) * sizeof(char);
    memmove(str->data + start_position + replacement.length, str->data + start_position + count, bytes_to_move);
    memcpy(str->data + start_position, replacement.data, replacement.length * sizeof(char));
    if (replacement.length > count)
    {
        str->length += replacement.length - count;
    }
    else
    {
        str->length -= count - replacement.length;
    }
}


CTD_String ctd_string_create_replace_all();

CTD_Strings ctd_string_split_inner(const CTD_String_View str, const CTD_String_View delimiter, Error* error)
{
    if (str.length == 0)
    {
        return (CTD_Strings) {0};
    }
    if (delimiter.length == 0)
    {
        return (CTD_Strings) {0};
    }

    ctd_internal_dynamic_array(CTD_String) strings = {.data = malloc(sizeof(CTD_String)), .length = 0, .capacity = 1};

    size_t starting_index = 0;
    option(size_t) ending_index;
    // This variable is here to run the loop one more time than otherwise in order to add the last string to our list
    bool done = false;
    while (!done)
    {
        ending_index = ctd_string_find(str, delimiter, starting_index, error);
        if (IS_NONE(ending_index))
        {
            done = true;
            ending_index.value = str.length;
        }
        CTD_String new_string = ctd_string_create_from_char_ptr(str.data + starting_index, ending_index.value - starting_index, error);
        if (error->error_type != NO_ERROR)
        {
            free(strings.data);
            return (CTD_Strings) {0};
        }
        ctd_internal_dynamic_array_append(strings, CTD_String, new_string, error);

        starting_index = ending_index.value + delimiter.length;
    }

    if (strings.length == 0)
    {
        free(strings.data);
        return (CTD_Strings) {0};
    }

    return (CTD_Strings) {.data = strings.data, .length = strings.length};
}

CTD_String_Views ctd_string_split_views_inner(const CTD_String_View str, const CTD_String_View delimiter, Error* error)
{
    if (str.length == 0)
    {
        return (CTD_String_Views) {0};
    }
    if (delimiter.length == 0)
    {
        return (CTD_String_Views) {0};
    }

    ctd_internal_dynamic_array(CTD_String_View) strings = {.data = malloc(sizeof(CTD_String_View)), .length = 0, .capacity = 1};

    size_t starting_index = 0;
    option(size_t) ending_index;
    // This variable is here to run the loop one more time than otherwise in order to add the last string to our list
    bool done = false;
    while (!done)
    {
        ending_index = ctd_string_find(str, delimiter, starting_index, error);
        if (IS_NONE(ending_index))
        {
            done = true;
            ending_index.value = str.length;
        }
        CTD_String_View new_string = to_view(str.data + starting_index, ending_index.value - starting_index);
        if (error->error_type != NO_ERROR)
        {
            free(strings.data);
            return (CTD_String_Views) {0};
        }
        ctd_internal_dynamic_array_append(strings, CTD_String_View, new_string, error);

        starting_index = ending_index.value + delimiter.length;
    }

    if (strings.length == 0)
    {
        free(strings.data);
        return (CTD_String_Views) {0};
    }

    return (CTD_String_Views) {.data = strings.data, .length = strings.length};
}

char* ctd_string_to_c_string_inner(CTD_String_View str, Error* error)
{
    char* c_string = malloc(str.length + 1);
    if (c_string == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Malloc Failed";

        return NULL;
    }
    memcpy(c_string, str.data, str.length * sizeof(char));
    c_string[str.length] = '\0';
    return c_string;
}

CTD_String ctd_string_remove_whitespace_inner(CTD_String_View str, Error* error)
{
    CTD_String modified_string = ctd_string_create(str.length, error);
    if (error->error_type != NO_ERROR)
    {
        return (CTD_String) {0};
    }
    size_t i = 0, j = 0;
    char expression_char;
    while (i < str.length)
    {
        expression_char = str.data[i];
        switch (expression_char)
        {
        case ' ':
        case '\f':
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        {
            break;
        }
        default:
        {
            modified_string.data[j] = expression_char;
            modified_string.length++;
            j++;
            break;
        }
        }
        i++;
    }

    return modified_string;
}

/**
 *
 */

/*
 * --------------------------------------------------------------------------------
 * | Private Functions                                                            |
 * --------------------------------------------------------------------------------
 */

/**
 * Expands a CTD_String if the string cannot fit a certain amount of characters.
 * The string's capacity will either be expanded by [capacity * 2 + 1], or [length + expand_by], choosing whichever
 * option is greater.
 *
 * @param str CTD_String to possibly be expanded.
 * @param expand_by The amount of characters that will be added to str.
 * @param error Pointer to Error struct.
 */
void ctd_string_maybe_expand(CTD_String* str, size_t expand_by, Error* error)
{
    if (str->length + expand_by >= str->capacity)
    {
        ctd_string_resize(str, max(str->length + expand_by, str->capacity * 2 + 1), error);
    }
}

/**
 * Contracts a CTD_String if the string isn't using a majority of it's capacity.
 * Specifically, it will contract to 1/2 of its current capacity if its length is less than 1/4 of its capacity.
 *
 * @param str
 * @param error
 */
void string_maybe_contract(CTD_String* str, Error* error)
{
    if (str->length < str->capacity / 4)
    {
        ctd_string_resize(str, str->capacity / 2, error);
    }
}

/**
 * Resizes the capacity of a CTD_String.
 *
 * @param str CTD_String to be resized.
 * @param new_capacity The new capacity of str.
 * @param error Pointer to Error struct.
 */
void ctd_string_resize(CTD_String* str, const size_t new_capacity, Error* error)
{
    char* new_data = realloc(str->data, new_capacity);
    if (new_data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Realloc Failed.";
        return;
    }
    str->capacity = new_capacity;
    str->data = new_data;
}


inline CTD_String_View ctd_string_view_create_from_full_string_(const CTD_String str)
{
    return (CTD_String_View) {.data = str.data, .length = str.length};
}

inline CTD_String_View ctd_string_view_create_from_view_(const CTD_String_View view)
{
    return view;
}

inline CTD_String_View ctd_string_view_create_from_full_c_string_(char* c_str)
{
    return (CTD_String_View) {.data = c_str, .length = strlen(c_str)};
}
