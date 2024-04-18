#ifndef CTD_MACRO_TOOLS_H
#define CTD_MACRO_TOOLS_H

#define defer(...) for (int _i = 1; _i; _i = 0, __VA_ARGS__)
#define EVAL(x) x

// https://stackoverflow.com/questions/29002373/best-way-to-deal-with-a-trailing-comma-when-using-x-macros-in-c
#define REMOVE_LEADING_COMMA(...) REMOVE_LEADING_COMMA_INNER(__VA_ARGS__)
#define REMOVE_LEADING_COMMA_INNER(discard, ...) __VA_ARGS__

#define macro_get_with_1_arguments(_1, MACRO, ...) MACRO
#define macro_get_with_2_arguments(_1, _2, MACRO, ...) MACRO
#define macro_get_with_3_arguments(_1, _2, _3, MACRO, ...) MACRO
#define macro_get_with_4_arguments(_1, _2, _3, _4, MACRO, ...) MACRO
#define macro_get_with_5_arguments(_1, _2, _3, _4, _5, MACRO, ...) MACRO
#define macro_get_with_6_arguments(_1, _2, _3, _4, _5, _6, MACRO, ...) MACRO
#define macro_get_with_7_arguments(_1, _2, _3, _4, _5, _6, _7, MACRO, ...) MACRO
#define macro_get_with_8_arguments(_1, _2, _3, _4, _5, _6, _7, _8, MACRO, ...) MACRO

#endif // MACRO_TOOLS_H
