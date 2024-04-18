#ifndef TEST_CTD_H
#define TEST_CTD_H
#include <stdio.h>

#define RUN_TEST(method_name, status_variable, number_of_tests_failed) \
printf("Test for %s: ", #method_name);\
status_variable = test_##method_name();\
if (status_variable)\
{\
    printf("\x1b[31m[Failed]\x1b[0m\n");                               \
    number_of_tests_failed++;                                          \
}\
else\
{\
    printf("\x1b[32m[Succeeded]\x1b[0m\n");\
}

#endif // TEST_CTD_H