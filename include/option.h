#ifndef OPTION_H
#define OPTION_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#define option(typename) option_##typename

#define OPTION_TYPE_DECL(type, typename)                                                                               \
    typedef struct option_##typename                                                                                   \
    {                                                                                                                  \
        type value;                                                                                                    \
        bool exists;                                                                                                   \
    }                                                                                                                  \
    option_##typename;

#define SOME(typename, value_param)                                                                                    \
    (option(typename))                                                                                                 \
    {                                                                                                                  \
        .value = value_param, .exists = true                                                                           \
    }
#define NONE(typename) (option(typename)){0};
#define IS_SOME(option_value) option_value.exists
#define IS_NONE(option_value) !option_value.exists
#define UNWRAP(option_value) option_value.exists ? option_value.value : abort(),

#define DEFAULT_OPTION_TYPES(X)                                                                                        \
    X(bool, bool)                                                                                                      \
    X(char, char)                                                                                                      \
    X(signed char, schar)                                                                                              \
    X(unsigned char, uchar)                                                                                            \
    X(short, short)                                                                                                    \
    X(signed short, sshort)                                                                                            \
    X(unsigned short, ushort)                                                                                          \
    X(int, int)                                                                                                        \
    X(unsigned int, uint)                                                                                              \
    X(long, long)                                                                                                      \
    X(signed long, slong)                                                                                              \
    X(unsigned long, ulong)                                                                                            \
    X(size_t, size_t)                                                                                                  \
    X(float, float)                                                                                                    \
    X(double, double)                                                                                                  \
    X(long double, ldouble)                                                                                            \
    X(uint8_t, uint8_t)                                                                                                \
    X(uint16_t, uint16_t)                                                                                              \
    X(uint32_t, uint32_t)                                                                                              \
    X(uint64_t, uint64_t)                                                                                              \
    X(uint_fast8_t, uint_fast8_t)                                                                                      \
    X(uint_fast16_t, uint_fast16_t)                                                                                    \
    X(uint_fast32_t, uint_fast32_t)                                                                                    \
    X(uint_fast64_t, uint_fast64_t)                                                                                    \
    X(uint_least8_t, uint_least8_t)                                                                                    \
    X(uint_least16_t, uint_least16_t)                                                                                  \
    X(uint_least32_t, uint_least32_t)                                                                                  \
    X(uint_least64_t, uint_least64_t)                                                                                  \
    X(uintmax_t, uintmax_t)                                                                                            \
    X(uintptr_t, uintptr_t)                                                                                            \
    X(int8_t, int8_t)                                                                                                  \
    X(int16_t, int16_t)                                                                                                \
    X(int32_t, int32_t)                                                                                                \
    X(int64_t, int64_t)                                                                                                \
    X(int_fast8_t, int_fast8_t)                                                                                        \
    X(int_fast16_t, int_fast16_t)                                                                                      \
    X(int_fast32_t, int_fast32_t)                                                                                      \
    X(int_fast64_t, int_fast64_t)                                                                                      \
    X(int_least8_t, int_least8_t)                                                                                      \
    X(int_least16_t, int_least16_t)                                                                                    \
    X(int_least32_t, int_least32_t)                                                                                    \
    X(int_least64_t, int_least64_t)                                                                                    \
    X(intmax_t, intmax_t)                                                                                              \
    X(intptr_t, intptr_t)
#endif // OPTION_H
