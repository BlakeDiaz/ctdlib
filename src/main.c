#include "error_message.h"
#include <stdio.h>
#include <ctd_string.h>

int main()
{
    Error error = {0};
    CTD_String str = ctd_string_create_from_c_string("Hi \tthe\vre John!\n", &error);

    CTD_String copy = ctd_string_remove_whitespace(str, &error);
    ERROR_ABORT(error);
    printf("Original: %.*s", (int)str.length, str.data);
    printf("Copy: %.*s\n", (int)copy.length, copy.data);

    ctd_string_destroy(&str);
    return 0;
}
