# CTD-Lib
A C utility library designed to provide functionality present in other languages' standard libraries, such as strings, data structures, and allocators.

## Features

### Allocators
*ctd_allocator.h*

A consistent allocator interface consisting of three function pointers and a context pointer:

- `void* context` - the internal data that the allocator keeps track of.
- `allocate(void* allocator_context, ptrdiff_t size, ptrdiff_t align)` - allocates a block of memory
- `reallocate(void* allocator_context, void* source, ptrdiff_t old_size, ptrdiff_t new_size, ptrdiff_t align)` - reallocates a block of memory
- `deallocate(void* allocator_context, void* source, ptrdiff_t size)` - frees a block of memory

To use a `ctd_allocator`, simply call one of the three functions from the allocator and pass the allocator's context pointer as the first parameter.

**Usage:**
```c
#include <ctd_allocator.h>
#include <stddef.h>

void do_work(ptrdiff_t size, ctd_allocator allocator)
{
    char* array = allocator.allocate(allocator.context, size, alignof(char));
    
    // Do work
    
    char* expanded_array = allocator.reallocate(allocator.context, array, size, size * 2, alignof(char));
    if (expanded_array == NULL)
    {
        allocator.free(allocator.context, array, size);
        
        return;
    }
    
    // Do work
    
    allocator.free(allocator.context, expanded_array, size);
}
```
#### Heap Allocators
*ctd_allocator.h*

A wrapper around `malloc`, `realloc`, and `free` that works under the `ctd_allocator` interface
#### Arena Allocators
*ctd_arena_allocator.h*

Allocators that hold a fixed chunk of memory and give memory from that fixed chunk.

Note - call `ctd_arena_allocator_destroy` instead of using `allocator.free` once you're completely done with the memory inside of the arena.
#### Page Allocators
*ctd_page_allocator.h*

These function similarly to arena allocators, but they can be expanded. Under the hood, they act as multiple arena allocators in a linked list, so you get the advantages of memory being close together, but it can grow as needed.

Note - call `ctd_page_allocator_destroy` instead of using `allocator.free` once you're completely done with the memory inside of the arena.
### Strings
*ctd_string.h*

Strings are represented by two different structs: `ctd_string`, a length-based view into a sequence of `char`s, and `ctd_string_builder`, a dynamic array of `char`s.

**Utility Functions for `ctd_string`:**
```c
ctd_string ctd_string_create(ptrdiff_t length, ctd_allocator allocator, ctd_error* error);
/*
* Macro that creates a ctd_string from a string literal.
*/
ctd_string_create_from_literal(literal);
/*
* Creates a ctd_string that spans two character pointers. End is exclusive.
*/
ctd_string ctd_string_span(char *beginning, char *end);
bool ctd_string_equals(ctd_string a, ctd_string b);
/*
* Works the same as string comparison functions in other languages.
*/
ptrdiff_t ctd_string_compare(ctd_string a, ctd_string b);
/*
* Finds the first instance of a substring in a ctd_string
*/
ctd_option(ptrdiff_t) ctd_string_find(ctd_string str, ctd_string substring, ptrdiff_t start, ctd_error* error);
/*
* Finds the last instance of a substring in a ctd_string
*/
ctd_option(ptrdiff_t) ctd_string_reverse_find(ctd_string str, ctd_string substring, ptrdiff_t end, ctd_error* error);
uint64_t ctd_string_hash(ctd_string str);
ctd_string ctd_string_remove_whitespace(ctd_string str, ctd_allocator allocator, ctd_error* error);
ctd_string ctd_string_copy(ctd_string str, ctd_allocator allocator, ctd_error* error);
char* ctd_string_to_c_string(ctd_string str, ctd_allocator allocator, ctd_error* error);
void ctd_string_destroy(ctd_string* self, ctd_allocator allocator);
```

**Utility Functions for `ctc_string_builder`:**
```c
ctd_string_builder ctd_string_builder_create(ptrdiff_t capacity, ctd_allocator* allocator, ctd_error* error);
/*
* Appends character to end of string builder.
*/
void ctd_string_builder_push_back(ctd_string_builder* self, char c, ctd_error* error);
/*
* Removes character from end of string builder.
*/
void ctd_string_builder_pop_back(ctd_string_builder* self, ctd_error* error);
void ctd_string_builder_append(ctd_string_builder *self, ctd_string str, ctd_error* error);
void ctd_string_builder_insert(ctd_string_builder *self, ctd_string str, ptrdiff_t index, ctd_error* error);
/*
* Removes length characters from the string builder starting at index
*/
void ctd_string_builder_remove(ctd_string_builder* self, ptrdiff_t index, ptrdiff_t length, ctd_error* error);
/*
* Replaces length characters starting at index with replacement
*/
void ctd_string_builder_replace_at(ctd_string_builder* self, ctd_string replacement, ptrdiff_t index, ptrdiff_t length, ctd_error* error);
ctd_option(ptrdiff_t) ctd_string_builder_find(ctd_string_builder* self, ctd_string substring, ptrdiff_t start, ctd_error* error);
ctd_option(ptrdiff_t) ctd_string_builder_reverse_find(ctd_string_builder* self, ctd_string substring, ptrdiff_t end, ctd_error* error);
bool ctd_string_builder_contains(ctd_string_builder* self, ctd_string substring, ctd_error* error);
/*
* Replaces first instance of substring with replacement, beggining at start
*/
void ctd_string_builder_replace(ctd_string_builder* self, ctd_string substring, ctd_string replacement, ptrdiff_t start, ctd_error* error);
/*
* Replaces every instance of substring with replacement, beggining at start
*/
void ctd_string_builder_replace_all(ctd_string_builder* self, ctd_string substring, ctd_string replacement, ptrdiff_t start, ctd_error* error);
void ctd_string_builder_reverse(ctd_string_builder* self);
void ctd_string_builder_clear(ctd_string_builder* self);
ctd_string ctd_string_builder_to_substring(ctd_string_builder *self, ptrdiff_t index, ptrdiff_t length,
                                           ctd_allocator allocator, ctd_error* error);
ctd_string ctd_string_builder_to_string(ctd_string_builder *self, ctd_allocator allocator, ctd_error* error);
ctd_string ctd_string_builder_to_span(ctd_string_builder *self, ptrdiff_t start, ptrdiff_t end, ctd_error* error);
void ctd_string_builder_destroy(ctd_string_builder *self);
```
### Generic Data Structures

Generic data structures are implemented using a 'template' based approach with macros.
There are three macros used:
- `CTD_STRUCTURE_NAME_TYPE_DECL(type, typename)` - Inserts a type declaration of the data structure
- `CTD_STRUCTURE_NAME_FUNCTIONS_DECL(type, typename)` - Inserts declarations of the functions used with the data structure
- `CTD_STRUCTURE_NAME_FUNCTIONS_IMPL(type, typename)` - Inserts definitions of the functions used with the data structure

Note - replace `STRUCTURE_NAME` with the name of the data structure you are using e.g. `STACK`

Note that the type and functions are named based off of `typename`.

`CTD_STRUCTURE_NAME_TYPE_DECL` and `CTD_STRUCTURE_NAME_FUNCTIONS_DECL` should only be declared once in a translation unit per typename to avoid duplicate type definitions or function declarations.
This can be done by creating a specific header file where these two macros are called once for each typename.

`CTD_STRUCTURE_NAME_TYPE_IMPL` should only be declared once in a program to avoid duplicate function definitions.
This can be done by creating a specific source file where this macro is called once for each typename.

If you want type inferring functions, you can use a macro 'hook' that you declare as follows:
```c
CTD_STRUCTURE_NAME_TYPES(X, ...)\
X(type1, typename1, __VA_ARGS__)\
X(type2, typename2, __VA_ARGS__)\
X(type3, typename3, __VA_ARGS__)
```
Note - replace `STRUCTURE_NAME` with the name of the data structure you are using e.g. `STACK`
You can use as many types as you want here.

After declaring this macro 'hook', you can call every function except `ctd_structure_name_create` without having to specify the typename.
For example:
`ctd_dynamic_array_int_append(parameters here)` becomes `ctd_dynamic_array_append(parameters here)`

#### Dynamic Arrays
*ctd_generic_dynamic_array.h*

A standard dynamic array.

**Usage:**
```c
#include <ctd_generic_dynamic_array.h>

CTD_DYNAMIC_ARRAY_TYPE_DECL(int, int)
CTD_DYNAMIC_ARRAY_FUNCTIONS_DECL(int, int)
CTD_DYNAMIC_ARRAY_IMPL(int, int)

int main(void)
{
    ctd_error error = {0};
    ctd_dynamic_array_int int_arr = ctd_dynamic_array_int_create(5 * sizeof(int), &error);
    for (int i = 0; i < 10; i++)
    {
        ctd_dynamic_array_int_append(&int_arr, i, &error);
    }

    for (int i = 0; i < int_arr.length; i++)
    {
        printf("Number: %d\n", int_arr.data[i]);
    }

    ctd_dynamic_array_int_destroy(&int_arr, &error);

    return 0;
}
```

**Hook Usage:**
```c
#include <ctd_generic_dynamic_array.h>

#define CTD_DYNAMIC_ARRAY_TYPES(X, ...)\
X(int, int, __VA_ARGS__)\
X(char, char, __VA_ARGS__)

// The following three statements are a convenient way to declare and implement each dynamic array type using the hook.
// It's not necessary to do this, it just makes it easier if there are multiple types.

CTD_DYNAMIC_ARRAY_TYPES(CTD_DYNAMIC_ARRAY_TYPE_DECL)
CTD_DYNAMIC_ARRAY_TYPES(CTD_DYNAMIC_ARRAY_FUNCTIONS_DECL)
CTD_DYNAMIC_ARRAY_TYPES(CTD_DYNAMIC_ARRAY_IMPL)

int main(void)
{
    ctd_error error = {0};
    ctd_dynamic_array_int int_arr = ctd_dynamic_array_int_create(5 * sizeof(int), &error);
    ctd_dynamic_array_char char_arr = ctd_dynamic_array_char_create(5 * sizeof(char), &error);
    ctd_dynamic_array_append(&int_arr, 5, &error);
    ctd_dynamic_array_append(&char_arr, 5, &error);

    printf("Int number: %d\n", int_arr.data[0]);
    printf("Char number: %d\n", char_arr.data[0]);

    ctd_dynamic_array_destroy(&int_arr, &error);
    ctd_dynamic_array_destroy(&char_arr, &error);

    return 0;
}
```

**Hook Usage**:
#### Stacks
*ctd_generic_stack.h*

A stack that uses a dynamic array under the hood.

### Options
*ctd_option.h*

Inspired by Rust's `std::option`, options are represented by a struct `ctd_option` that contains an `exists` field for if a value exists, and a `value` field. Options leverage a similar 'template' macro system as the generic data structures do, but they don't use the same type-inference hooks.

**Usage:**
```c
#include <stddef.h>
#include <option.h>

// Type, typename
CTD_OPTION_TYPE_DECL(ptrdiff_t, size_type)
 
ctd_option_size_type find_character(char* array, ptrdiff_t length, char c)
{
    for (ptrdiff_t i = 0; i < length; i++)
    {
        if (array[i] == c)
        {
            return SOME(size_type, i);
        }
        
        return NONE(size_type);
    }
}

int main(void)
{
    char array = ['a', 'b', 'c', 'd', 'e'];
    
    ctd_option_size_type option = find_character(array, 5, 'd');
    if (option.exists)
    {
        // Do work
    }
    else
    {
        return 1;
    }
    
    return 0;
}
```
### Errors
*ctd_error.h*

Errors are represented by a simple struct `ctd_error` that has two members: an enum representing the type of error, and a const char* to hold an error message with.

**Usage:**
```c
#include <stddef.h>
#include <ctd_error.h>

int* create_array(size_t size, ctd_error* error)
{
    if (size > 50)
    {
        error->error_type = INVALID_ARGUMENT;
        error->error_message = "Error: size must be under 50 in create_array function.";
        
        return NULL;
    }
    int* array = malloc(size * sizeof(int));
    if (array == NULL)
    {
        error->error_type = ALLOCATION_FAIL;
        error->error_message = "Error: Failed to allocate array in create_array function.";
        
        return NULL;
    }
    
    for (size_t i = 0; i < size; i++)
    {
        array[i] = i * i;	
    }
    
    return array;
}
```
## Planned Features
- More data structures
    - Linked lists
    - Queues
    - Hash maps