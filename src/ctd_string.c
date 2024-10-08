#include <ctd_string.h>
#include <stdalign.h>
#include <string.h>

void ctd_string_builder_maybe_expand(ctd_string_builder* self, ptrdiff_t expand_by, ctd_error* error);
void ctd_string_builder_maybe_contract(ctd_string_builder* self, ctd_error* error);
void ctd_string_builder_resize(ctd_string_builder *self, ptrdiff_t new_capacity, ctd_error* error);

/**
 * Creates an empty string with a given length/capacity.
 *
 * @param length
 * @param allocator
 * @param error
 * @return
 */
ctd_string ctd_string_create(ptrdiff_t length, ctd_allocator allocator, ctd_error* error)
{
    char* data = allocator.allocate(allocator.context, length * sizeof(char), alignof(char));
    if (data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Allocation of ctd_string failed.";

        return (ctd_string) {0};
    }

    return (ctd_string) {.data = data, .length = length};
}

ctd_string ctd_string_span(char *beginning, char *end)
{
    return (ctd_string) {.data = beginning, .length = end - beginning};
}

bool ctd_string_equals(ctd_string a, ctd_string b)
{
    if (a.length != b.length)
    {
        return false;
    }
    for (ptrdiff_t i = 0; i < a.length; i++)
    {
        if (a.data[i] != b.data[i])
        {
            return false;
        }
    }
    return true;
}

ptrdiff_t ctd_string_compare(ctd_string a, ctd_string b)
{
    size_t min_length = ctd_min(a.length, b.length);
    for (ptrdiff_t i = 0; i < min_length; i++)
    {
        if (a.data[i] != b.data[i])
        {
            return a.data[i] - b.data[i];
        }
    }

    return a.length - b.length;
}

ctd_option(ptrdiff_t) ctd_string_find(ctd_string str, ctd_string substring, ptrdiff_t start, ctd_error* error)
{
    if (start >= str.length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Starting index was greater than or equal to string's length.";

        return NONE(ptrdiff_t);
    }

    if (str.length - start < substring.length)
    {
        return NONE(ptrdiff_t);
    }
    if (str.length == 0 || substring.length == 0)
    {
        return NONE(ptrdiff_t);
    }

    for (ptrdiff_t i = start; i <= str.length - substring.length; i++)
    {
        // If this is true, there isn't enough room inside str to contain the substring
        if (str.length - i < substring.length)
        {
            return NONE(ptrdiff_t);
        }
        // If this is true, then the substring can't start at this character, and we move onto the next one
        if (str.data[i] != substring.data[0])
        {
            continue;
        }

        ctd_string candidate = {.data = str.data + i, .length = substring.length};
        if (ctd_string_equals(candidate, substring))
        {
            return SOME(ptrdiff_t, i);
        }
    }

    return NONE(ptrdiff_t);
}

/**
 * Finds the last instance of a substring in a string.
 *
 * @param str String to be searched.
 * @param substring Substring to be found.
 * @param end Where search ends. Exclusive.
 * @param error Pointer to error struct.
 * @return
 */
ctd_option(ptrdiff_t) ctd_string_reverse_find(ctd_string str, ctd_string substring, ptrdiff_t end, ctd_error* error)
{
    if (end > str.length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "End was greater than string length in ctd_string_reverse_find";

        return NONE(ptrdiff_t);
    }

    if (end < substring.length)
    {
        return NONE(ptrdiff_t);
    }
    if (str.length == 0 || substring.length == 0)
    {
        return NONE(ptrdiff_t);
    }
    ptrdiff_t beginning_index = end - substring.length;

    for (ptrdiff_t i = beginning_index; i >= 0; i--)
    {
        if (str.data[i] != substring.data[0])
        {
            continue;
        }

        ctd_string candidate = {.data = str.data + i, .length = substring.length};
        if (ctd_string_equals(candidate, substring))
        {
            return SOME(ptrdiff_t, i);
        }
    }

    return NONE(ptrdiff_t);
}

uint64_t ctd_string_hash(ctd_string str)
{
    unsigned long hash = 5381;
    int c;

    for (ptrdiff_t i = 0; i < str.length; i++)
    {
        c = str.data[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

ctd_string ctd_string_remove_whitespace(ctd_string str, ctd_allocator allocator, ctd_error* error)
{
    char* initial_buffer = allocator.allocate(allocator.context, str.length * sizeof(char), alignof(char));
    if (initial_buffer == NULL)
    {
        return (ctd_string) {0};
    }
    ptrdiff_t length = 0, i = 0, j = 0;
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
            initial_buffer[j] = expression_char;
            length++;
            j++;
            break;
        }
        }
        i++;
    }
    ctd_string modified_string = ctd_string_create(length, allocator, error);
    if (error->error_type != NO_ERROR)
    {
        allocator.deallocate(allocator.context, initial_buffer, str.length);
        return (ctd_string) {0};
    }

    memcpy(modified_string.data, initial_buffer, length * sizeof(char));
    allocator.deallocate(allocator.context, initial_buffer, str.length);

    return modified_string;
}


ctd_string ctd_string_copy(ctd_string str, ctd_allocator allocator, ctd_error* error)
{
    char *data = allocator.allocate(allocator.context, str.length * sizeof(char), alignof(char));
    if (data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Allocation of ctd_string failed.";

        return (ctd_string) {0};
    }
    memcpy(data, str.data, str.length);

    return (ctd_string){.data = data, .length = str.length};
}

char* ctd_string_to_c_string(ctd_string str, ctd_allocator allocator, ctd_error* error)
{
    char* data = allocator.allocate(allocator.context, (str.length + 1) * sizeof(char), alignof(char));
    if (data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Allocation of ctd_string failed.";

        return NULL;
    }
    memcpy(data, str.data, str.length);
    data[str.length] = '\0';

    return data;
}

void ctd_string_destroy(ctd_string* self, ctd_allocator allocator)
{
    allocator.deallocate(allocator.context, self->data, self->length * sizeof(char));
    *self = (ctd_string) {0};
}

ctd_string_builder ctd_string_builder_create(ptrdiff_t capacity, ctd_allocator* allocator, ctd_error* error)
{
    char *data = allocator->allocate(allocator->context, capacity, alignof(char));
    if (data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Allocation of ctd_string_builder failed.";

        return (ctd_string_builder) {0};
    }

    return (ctd_string_builder){.data = data, .length = 0, .capacity = capacity, .allocator = allocator};
}

void ctd_string_builder_push_back(ctd_string_builder* self, char c, ctd_error* error)
{
    if (self == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Ctd_string_builder was NULL";

        return;
    }
    ctd_string_builder_maybe_expand(self, 1, error);
    if (error->error_type != NO_ERROR)
    {
        return;
    }
    self->data[self->length] = c;
    self->length++;
}

void ctd_string_builder_pop_back(ctd_string_builder* self, ctd_error* error)
{
    if (self == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Ctd_string_builder was NULL";

        return;
    }
    if (self->length == 0)
    {
        return;
    }

    self->data[self->length - 1] = 0;
    self->length--;
    ctd_string_builder_maybe_contract(self, error);
}

void ctd_string_builder_append(ctd_string_builder *self, ctd_string str, ctd_error* error)
{
    if (self == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Ctd_string_builder was NULL";

        return;
    }
    ctd_string_builder_maybe_expand(self, str.length, error);
    if (error->error_type != NO_ERROR)
    {
        return;
    }

    memcpy(self->data + self->length, str.data, str.length);
    self->length += str.length;
}

void ctd_string_builder_insert(ctd_string_builder *self, ctd_string str, ptrdiff_t index, ctd_error* error)
{
    if (self == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Ctd_string_builder was NULL";

        return;
    }
    if (index > self->length)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Index to append at was > ctd_string_builder's length";
    }
    if (index == self->length)
    {
        ctd_string_builder_append(self, str, error);
        return;
    }

    ctd_string_builder_maybe_expand(self, str.length, error);
    if (error->error_type != NO_ERROR)
    {
        return;
    }
    memmove(self->data + index + str.length, self->data + index, (self->length - index) * sizeof(char));
    memcpy(self->data + index, str.data, str.length * sizeof(char));

    self->length += str.length;
}

void ctd_string_builder_remove(ctd_string_builder* self, ptrdiff_t index, ptrdiff_t length, ctd_error* error)
{
    if (self == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Ctd_string_builder was NULL";

        return;
    }

    ptrdiff_t bytes_to_move = (self->length - index - length) * sizeof(char);
    if (bytes_to_move < 0)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Index + length > ctd_string_builder's length in remove method.";

        return;
    }

    memset(self->data + index, 0, length * sizeof(char));
    memmove(self->data + index, self->data + index + length, bytes_to_move);
    self->length -= length;
    ctd_string_builder_maybe_contract(self, error);
}

/**
 * Replace a sequence of characters with a string.
 *
 * @param self String builder.
 * @param replacement String to be inserted.
 * @param index Starting index of sequence of characters to be removed. Inclusive.
 * @param length Number of characters to be removed.
 * @param error Pointer to error struct.
 */
void ctd_string_builder_replace_at(ctd_string_builder* self, ctd_string replacement, ptrdiff_t index, ptrdiff_t length, ctd_error* error)
{
    if (self == NULL)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Ctd_string_builder was NULL";

        return;
    }

    ptrdiff_t movement_index = index + length;
    ptrdiff_t bytes_to_move = (self->length - index - length) * sizeof(char);
    if (bytes_to_move < 0)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Index + length > ctd_string_builder's length in remove method.";

        return;
    }

    ptrdiff_t length_difference = replacement.length - length;
    ctd_string_builder_maybe_expand(self, length_difference, error);
    if (error->error_type != NO_ERROR)
    {
        return;
    }

    memmove(self->data + movement_index + length_difference, self->data + movement_index, bytes_to_move);
    memcpy(self->data + index, replacement.data, replacement.length * sizeof(char));
    self->length += length_difference;
}

/**
 *
 * @param self String builder to search.
 * @param substring String to be searched for.
 * @param start Beginning index to search from. Inclusive.
 * @param error Pointer to error struct.
 * @return
 */
ctd_option(ptrdiff_t) ctd_string_builder_find(ctd_string_builder* self, ctd_string substring, ptrdiff_t start, ctd_error* error)
{
    ctd_string str= {.data = self->data, .length = self->length};

    return ctd_string_find(str, substring, start, error);
}

/**
 *
 * @param self String builder to search.
 * @param substring String to be searched for.
 * @param end Index that searching won't go past. Exclusive.
 * @param error Pointer ot error struct.
 * @return
 */
ctd_option(ptrdiff_t) ctd_string_builder_reverse_find(ctd_string_builder* self, ctd_string substring, ptrdiff_t end, ctd_error* error)
{
    ctd_string str= {.data = self->data, .length = self->length};

    return ctd_string_reverse_find(str, substring, end, error);
}

bool ctd_string_builder_contains(ctd_string_builder* self, ctd_string substring, ctd_error* error)
{
    return IS_SOME(ctd_string_builder_find(self, substring, 0, error));
}

void ctd_string_builder_replace(ctd_string_builder* self, ctd_string substring, ctd_string replacement, ptrdiff_t start, ctd_error* error)
{
    ctd_option(ptrdiff_t) index_option = ctd_string_builder_find(self, substring, start, error);
    if (error->error_type != NO_ERROR)
    {
        return;
    }
    if (IS_NONE(index_option))
    {
        return;
    }
    ptrdiff_t index = index_option.value;
    ctd_string_builder_replace_at(self, replacement, index, substring.length, error);
}

void ctd_string_builder_replace_all(ctd_string_builder* self, ctd_string substring, ctd_string replacement, ptrdiff_t start, ctd_error* error)
{
    ctd_option(ptrdiff_t) index_option = ctd_string_builder_find(self, substring, start, error);
    ptrdiff_t length_difference = replacement.length - substring.length;
    ptrdiff_t index;
    while (IS_SOME((index_option = ctd_string_builder_find(self, substring, start, error))) && error->error_type == NO_ERROR)
    {
        index = index_option.value;
        ctd_string_builder_replace_at(self, replacement, index, substring.length, error);
        start = index + replacement.length;
        if (start >= self->length + length_difference)
        {
            break;
        }
    }
}

void ctd_string_builder_reverse(ctd_string_builder* self)
{
    ptrdiff_t i = 0;
    ptrdiff_t opposite_index = self->length - 1;
    char temp;
    while(i < opposite_index)
    {
        temp = self->data[i];
        self->data[i] = self->data[opposite_index];
        self->data[opposite_index] = temp;

        i++;
        opposite_index--;
    }
}

void ctd_string_builder_clear(ctd_string_builder* self)
{
    memset(self->data, 0, self->length * sizeof(char));
    self->length = 0;
}


ctd_string ctd_string_builder_to_substring(ctd_string_builder *self, ptrdiff_t index, ptrdiff_t length,
                                           ctd_allocator allocator, ctd_error* error)
{
    char *data = allocator.allocate(allocator.context, length, alignof(char));
    if (data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Allocation of ctd_string failed.";

        return (ctd_string) {0};
    }
    memcpy(data, self->data + index, length * sizeof(char));

    return (ctd_string){.data = data, .length = length};
}

ctd_string ctd_string_builder_to_string(ctd_string_builder *self, ctd_allocator allocator, ctd_error* error)
{
    char *data = allocator.allocate(allocator.context, self->length, alignof(char));
    if (data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Allocation of ctd_string failed.";

        return (ctd_string) {0};
    }
    memcpy(data, self->data, self->length * sizeof(char));

    return (ctd_string){.data = data, .length = self->length};
}

/**
 * Returns a span
 * Note - if start == end, returns an empty string.
 *
 * @param self
 * @param start Inclusive.
 * @param end Exclusive.
 * @param error
 * @return
 */
ctd_string ctd_string_builder_to_span(ctd_string_builder *self, ptrdiff_t start, ptrdiff_t end, ctd_error* error)
{
    if (start < 0 || end > self->length || start > end)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Invalid start and/or end index for ctd_string_builder_to_span.";

        return (ctd_string) {0};
    }
    if (start == end)
    {
        return (ctd_string) {0};
    }

    return (ctd_string) {.data = self->data + start, .length = end - start};
}

/**
 * Destroys and frees a string builder.
 * Note that the allocator itself is not destroyed. Make sure to keep a reference on hand if you need to use it after.
 *
 * @param self String builder to be destroyed.
 */
void ctd_string_builder_destroy(ctd_string_builder *self)
{
    self->allocator->deallocate(self->allocator->context, self->data, self->capacity);
    *self = (ctd_string_builder) {0};
}

void ctd_string_builder_maybe_expand(ctd_string_builder* self, ptrdiff_t expand_by, ctd_error* error)
{
    if (self->length + expand_by >= self->capacity)
    {
        ctd_string_builder_resize(self, ctd_max(self->length + expand_by, self->capacity * 2), error);
    }
}

void ctd_string_builder_maybe_contract(ctd_string_builder* self, ctd_error* error)
{
    if (self->length <= self->capacity / 4)
    {
        ctd_string_builder_resize(self, self->capacity / 2, error);
    }
}

void ctd_string_builder_resize(ctd_string_builder *self, ptrdiff_t new_capacity, ctd_error* error)
{
    char* new_data = self->allocator->reallocate(self->allocator->context, self->data, self->capacity, new_capacity, alignof(char));
    if (new_data == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Failed to realloc ctd_string_builder";

        return;
    }

    self->capacity = new_capacity;
    self->data = new_data;
}