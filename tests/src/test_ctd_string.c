#include <ctd_string.h>
#include <string.h>
#include <test.h>

static int test_ctd_string_create_from_literal()
{
    ctd_string str_1 = ctd_string_create_from_literal("Hello there!");
    char* literal = "Hello there!";
    ptrdiff_t length = (sizeof("Hello there!")/sizeof(char)) - 1;

    if (str_1.length != length) return 1;

    for (ptrdiff_t i = 0; i < length; i++)
    {
        if (str_1.data[i] != literal[i]) return 1;
    }

    return 0;
}

static int test_ctd_string_span()
{
    ctd_string str = ctd_string_create_from_literal("Hi there!");
    ctd_string span = ctd_string_span(str.data + 3, str.data + str.length - 1);
    if (span.data[0] != str.data[3])
    {
        return 1;
    }
    if (span.length != 5)
    {
        return 1;
    }

    return 0;
}

static int test_ctd_string_equals()
{
    ctd_string str_1 = ctd_string_create_from_literal("Hello there!");
    ctd_string str_2 = ctd_string_create_from_literal("Hello there!");
    ctd_string str_3 = ctd_string_create_from_literal("Hello there");
    ctd_string str_4 = ctd_string_create_from_literal("ello there!");
    ctd_string str_5 = ctd_string_create_from_literal("Hellothere");
    ctd_string str_6 = ctd_string_create_from_literal("");
    ctd_string str_7 = ctd_string_create_from_literal("");

    if (!ctd_string_equals(str_1, str_2)) return 1;
    if (!ctd_string_equals(str_2, str_1)) return 1;
    if (!ctd_string_equals(str_2, str_2)) return 1;
    if (ctd_string_equals(str_1, str_3)) return 1;
    if (ctd_string_equals(str_1, str_4)) return 1;
    if (ctd_string_equals(str_1, str_5)) return 1;
    if (ctd_string_equals(str_1, str_6)) return 1;
    if (!ctd_string_equals(str_6, str_7)) return 1;

    return 0;
}

static int test_ctd_string_compare()
{
    ctd_string str_1 = ctd_string_create_from_literal("Hi there!");
    ctd_string str_2 = ctd_string_create_from_literal("Hi there!-");
    ctd_string str_3 = ctd_string_create_from_literal("Ai there!");
    ctd_string str_4 = ctd_string_create_from_literal("Ai ");
    ctd_string str_5 = ctd_string_create_from_literal("Aaaaaaaaaaaaaaaaa");
    ctd_string str_6 = ctd_string_create_from_literal("");

    if (ctd_string_compare(str_1, str_2) >= 0) return 1;
    if (ctd_string_compare(str_2, str_1) < 0) return 1;
    if (ctd_string_compare(str_1, str_3) <= 0) return 1;
    if (ctd_string_compare(str_3, str_4) <= 0) return 1;
    if (ctd_string_compare(str_4, str_5) <= 0) return 1;
    if (ctd_string_compare(str_6, str_5) > 0) return 1;

    return 0;
}

static int test_ctd_string_find()
{
    Error error = {0};
    ctd_string str_1 = ctd_string_create_from_literal("Hi there!");
    ctd_string to_find = ctd_string_create_from_literal("e");

    option(ptrdiff_t) index_option = ctd_string_find(str_1, to_find, 0, &error);
    if (error.error_type != NO_ERROR) return 1;
    if (IS_NONE(index_option)) return 1;
    if (index_option.value != 5) return 1;

    index_option = ctd_string_find(str_1, to_find, 6, &error);
    if (error.error_type != NO_ERROR) return 1;
    if (IS_NONE(index_option)) return 1;
    if (index_option.value != 7) return 1;

    return 0;
}

static int test_ctd_string_reverse_find()
{
    Error error = {0};
    ctd_string str_1 = ctd_string_create_from_literal("Hi there!");
    ctd_string to_find = ctd_string_create_from_literal("e");

    option(ptrdiff_t) index_option = ctd_string_reverse_find(str_1, to_find, str_1.length, &error);
    if (error.error_type != NO_ERROR) return 1;
    if (IS_NONE(index_option)) return 1;
    if (index_option.value != 7) return 1;

    index_option = ctd_string_reverse_find(str_1, to_find, 6, &error);
    if (error.error_type != NO_ERROR) return 1;
    if (IS_NONE(index_option)) return 1;
    if (index_option.value != 5) return 1;

    return 0;
}

static int test_ctd_string_remove_whitespace()
{
    Error error = {0};
    ctd_default_allocator allocator = ctd_default_allocator_create();
    ctd_string str_1 = ctd_string_create_from_literal("\nHi there!\n How are you\t today?\n");
    ctd_string str_2 = ctd_string_create_from_literal("Hithere!Howareyoutoday?");

    ctd_string str_without_whitespace = ctd_string_remove_whitespace(str_1, allocator.allocator, &error);
    if (error.error_type != NO_ERROR) return 1;
    if (str_without_whitespace.length != str_2.length)
    {
        ctd_string_destroy(&str_without_whitespace, allocator.allocator);
        return 1;
    }
    for (ptrdiff_t i = 0; i < str_2.length; i++)
    {
        if (str_without_whitespace.data[i] != str_2.data[i])
        {
            ctd_string_destroy(&str_without_whitespace, allocator.allocator);
            return 1;
        }
    }

    ctd_string_destroy(&str_without_whitespace, allocator.allocator);
    return 0;
}

static int test_ctd_string_copy()
{
    Error error = {0};
    ctd_default_allocator default_allocator = ctd_default_allocator_create();

    ctd_string a = ctd_string_create_from_literal("Hi there!");
    ctd_string b = ctd_string_copy(a, default_allocator.allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        return 1;
    }
    if (!ctd_string_equals(a, b))
    {
        ctd_string_destroy(&b, default_allocator.allocator);

        return 1;
    }
    if (a.data == b.data) return 1;

    ctd_string_destroy(&b, default_allocator.allocator);
    return 0;
}

static int test_ctd_string_to_c_string()
{
    Error error = {0};
    ctd_default_allocator default_allocator = ctd_default_allocator_create();

    ctd_string str = ctd_string_create_from_literal("Hi there!");
    char* c_string = ctd_string_to_c_string(str, default_allocator.allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        return 1;
    }
    if (strlen(c_string) != str.length)
    {
        goto cleanup;
    }

    for (ptrdiff_t i = 0; i < str.length; i++)
    {
        if (str.data[i] != c_string[i])
        {
            goto cleanup;
        }
    }

    default_allocator.allocator.free(default_allocator.allocator.context, c_string, (str.length + 1) * sizeof(char));
    return 0;

cleanup:
    default_allocator.allocator.free(default_allocator.allocator.context, c_string, (str.length + 1) * sizeof(char));
    return 1;

}

static int test_ctd_string_builder_push_back()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        return 1;
    }
    char* data = "Hi there!";
    ptrdiff_t length = lengthof("Hi there!");
    memcpy(builder.data, data, length * sizeof(char));
    builder.length += length;

    ctd_string_builder_push_back(&builder, 'c', &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }

    if (builder.length != length + 1)
    {
        goto cleanup;
    }

    for (ptrdiff_t i = 0; i < length; i++)
    {
        if (builder.data[i] != data[i])
        {
            goto cleanup;
        }
    }
    if (builder.data[length] != 'c')
    {
        goto cleanup;
    }

    ctd_string_builder_destroy(&builder);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

static int test_ctd_string_builder_pop_back()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        return 1;
    }
    char* data = "Hi there!";
    ptrdiff_t length = lengthof("Hi there!");
    memcpy(builder.data, data, length * sizeof(char));
    builder.length += length;

    ctd_string_builder_pop_back(&builder, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    if (builder.length != length - 1)
    {
        goto cleanup;
    }
    for (ptrdiff_t i = 0; i < builder.length; i++)
    {
        if (builder.data[i] != data[i])
        {
            goto cleanup;
        }
    }

    ctd_string_builder_destroy(&builder);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

static int test_ctd_string_builder_append()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        return 1;
    }

    ctd_string str_1 = ctd_string_create_from_literal("Hello there!");
    ctd_string str_2 = ctd_string_create_from_literal(" Hi!");
    ctd_string_builder_append(&builder, str_1, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    if (builder.length != str_1.length)
    {
        goto cleanup;
    }
    for (ptrdiff_t i = 0; i < builder.length; i++)
    {
        if (builder.data[i] != str_1.data[i])
        {
            goto cleanup;
        }
    }

    ctd_string_builder_append(&builder, str_2, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    if (builder.length != str_1.length + str_2.length)
    {
        goto cleanup;
    }
    for (ptrdiff_t i = 0; i < str_1.length; i++)
    {
        if (builder.data[i] != str_1.data[i])
        {
            goto cleanup;
        }
    }
    for (ptrdiff_t i = 0; i < str_2.length; i++)
    {
        if (builder.data[str_1.length + i] != str_2.data[i])
        {
            goto cleanup;
        }
    }

    ctd_string_builder_destroy(&builder);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

static int test_ctd_string_builder_insert()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        return 1;
    }
    ctd_string str_1 = ctd_string_create_from_literal("Hi there!");
    ctd_string str_2 = ctd_string_create_from_literal("asdf");
    ctd_string str_3 = ctd_string_create_from_literal("asdfHi there!");
    ctd_string str_4 = ctd_string_create_from_literal("asdfHi asdfthere!");
    ctd_string str_5 = ctd_string_create_from_literal("asdfHi asdfthere!asdf");
    ctd_string_builder_append(&builder, str_1, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }

    ctd_string_builder_insert(&builder, str_2, 0, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (str_3.length != builder.length) goto cleanup;
    if (strncmp(str_3.data, builder.data, builder.length) != 0) goto cleanup;


    ctd_string_builder_insert(&builder, str_2, 7, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (str_4.length != builder.length) goto cleanup;
    if (strncmp(str_4.data, builder.data, builder.length) != 0) goto cleanup;


    ctd_string_builder_insert(&builder, str_2, builder.length, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (str_5.length != builder.length) goto cleanup;
    if (strncmp(str_5.data, builder.data, builder.length) != 0) goto cleanup;

    ctd_string_builder_destroy(&builder);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

static int test_ctd_string_builder_remove()
{

    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        return 1;
    }
    ctd_string str_1 = ctd_string_create_from_literal("asdfHi asdfthere!asdf");
    ctd_string str_2 = ctd_string_create_from_literal("asdfHi asdfthere!");
    ctd_string str_3 = ctd_string_create_from_literal("asdfHi there!");
    ctd_string str_4 = ctd_string_create_from_literal("Hi there!");
    ctd_string str_5 = ctd_string_create_from_literal("Hi there");
    ctd_string str = ctd_string_create_from_literal("asdf");

    ctd_string_builder_append(&builder, str_1, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }

    ctd_string_builder_remove(&builder, 17, str.length, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (str_2.length != builder.length) goto cleanup;
    if (strncmp(str_2.data, builder.data, builder.length) != 0) goto cleanup;

    ctd_string_builder_remove(&builder, 7, str.length, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (str_3.length != builder.length) goto cleanup;
    if (strncmp(str_3.data, builder.data, builder.length) != 0) goto cleanup;

    ctd_string_builder_remove(&builder, 0, str.length, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (str_4.length != builder.length) goto cleanup;
    if (strncmp(str_4.data, builder.data, builder.length) != 0) goto cleanup;

    ctd_string_builder_remove(&builder, builder.length-1, 1, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (str_5.length != builder.length) goto cleanup;
    if (strncmp(str_5.data, builder.data, builder.length) != 0) goto cleanup;

    ctd_string_builder_destroy(&builder);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

static int test_ctd_string_builder_find()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder_1 = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    ctd_string_builder builder_2 = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        return 1;
    }
    ctd_string str_1 = ctd_string_create_from_literal("eHello there!e");
    ctd_string str_2 = ctd_string_create_from_literal("gkceHellothegkce!egkc");
    ctd_string search_1 = ctd_string_create_from_literal("e");
    ctd_string search_2 = ctd_string_create_from_literal("gkc");
    ctd_string search_3 = ctd_string_create_from_literal("z");
    ctd_string_builder_append(&builder_1, str_1, &error);
    ctd_string_builder_append(&builder_2, str_2, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    option(ptrdiff_t) index_option = ctd_string_builder_find(&builder_1, search_1, 0, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (IS_NONE(index_option)) goto cleanup;
    if (index_option.value != 0) goto cleanup;

    index_option = ctd_string_builder_find(&builder_1, search_1, 1, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (index_option.value != 2) goto cleanup;

    index_option = ctd_string_builder_find(&builder_1, search_1, 3, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (index_option.value != 9) goto cleanup;

    index_option = ctd_string_builder_find(&builder_1, search_1, 10, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (index_option.value != 11) goto cleanup;

    index_option = ctd_string_builder_find(&builder_1, search_1, 13, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (index_option.value != 13) goto cleanup;

    index_option = ctd_string_builder_find(&builder_1, search_2, 0, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (!IS_NONE(index_option)) goto cleanup;

    index_option = ctd_string_builder_find(&builder_1, search_3, 0, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (!IS_NONE(index_option)) goto cleanup;

    index_option = ctd_string_builder_find(&builder_2, search_2, 0, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (index_option.value != 0 || IS_NONE(index_option)) goto cleanup;

    index_option = ctd_string_builder_find(&builder_2, search_2, 1, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (index_option.value != 12 ) goto cleanup;

    index_option = ctd_string_builder_find(&builder_2, search_2, 13, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (index_option.value != 18 ) goto cleanup;

    index_option = ctd_string_builder_find(&builder_2, search_2, 19, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    if (!IS_NONE(index_option)) goto cleanup;

    ctd_string_builder_destroy(&builder_1);
    ctd_string_builder_destroy(&builder_2);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder_1);
    ctd_string_builder_destroy(&builder_2);
    return 1;
}

static int test_ctd_string_builder_contains()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    ctd_string str = ctd_string_create_from_literal("Hi there!");
    ctd_string_builder_append(&builder, str, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    ctd_string str_1 = ctd_string_create_from_literal("Hi");
    ctd_string str_2 = ctd_string_create_from_literal("e");
    ctd_string str_3 = ctd_string_create_from_literal("Ghl");
    ctd_string str_4 = ctd_string_create_from_literal("z");
    ctd_string str_5 = {.data = NULL, .length = 0};

    bool result = ctd_string_builder_contains(&builder, str_1, &error);
    if (error.error_type != NO_ERROR || result == false) goto cleanup;

    result = ctd_string_builder_contains(&builder, str_2, &error);
    if (error.error_type != NO_ERROR || result == false) goto cleanup;

    result = !ctd_string_builder_contains(&builder, str_3, &error);
    if (error.error_type != NO_ERROR || result == false) goto cleanup;

    result = !ctd_string_builder_contains(&builder, str_4, &error);
    if (error.error_type != NO_ERROR || result == false) goto cleanup;

    result = !ctd_string_builder_contains(&builder, str_5, &error);
    if (error.error_type != NO_ERROR || result == false) goto cleanup;

    ctd_string_builder_destroy(&builder);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

static int test_ctd_string_builder_replace()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    ctd_string str = ctd_string_create_from_literal("Hi there! there is a thing over there");
    ctd_string search_1 = ctd_string_create_from_literal("there");
    ctd_string replace_1 = ctd_string_create_from_literal("hhhh");
    ctd_string compare_1 = ctd_string_create_from_literal("Hi hhhh! there is a thing over there");
    ctd_string compare_2 = ctd_string_create_from_literal("Hi hhhh! there is a thing over hhhh");
    ctd_string compare_3 = ctd_string_create_from_literal("Hi hhhh! hhhh is a thing over hhhh");
    ctd_string_builder_append(&builder, str, &error);
    if (error.error_type != NO_ERROR) goto cleanup;

    ctd_string_builder_replace(&builder, search_1, replace_1, 0, &error);
    if (error.error_type != NO_ERROR || !ctd_string_equals(ctd_string_builder_to_span(&builder, 0, builder.length, &error), compare_1)) goto cleanup;

    ctd_string_builder_replace(&builder, search_1, replace_1, 11, &error);
    if (error.error_type != NO_ERROR || !ctd_string_equals(ctd_string_builder_to_span(&builder, 0, builder.length, &error), compare_2)) goto cleanup;

    ctd_string_builder_replace(&builder, search_1, replace_1, 0, &error);
    if (error.error_type != NO_ERROR || !ctd_string_equals(ctd_string_builder_to_span(&builder, 0, builder.length, &error), compare_3)) goto cleanup;

    ctd_string_builder_replace(&builder, search_1, replace_1, 0, &error);
    if (error.error_type != NO_ERROR || !ctd_string_equals(ctd_string_builder_to_span(&builder, 0, builder.length, &error), compare_3)) goto cleanup;

    ctd_string_builder_destroy(&builder);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

static int test_ctd_string_builder_replace_all()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder_1 = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    ctd_string_builder builder_2 = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    ctd_string str = ctd_string_create_from_literal("Hi there! there is a thing over there");
    ctd_string search_1 = ctd_string_create_from_literal("there");
    ctd_string search_2 = ctd_string_create_from_literal("i");
    ctd_string replace_1 = ctd_string_create_from_literal("hhhh");
    ctd_string replace_2 = ctd_string_create_from_literal("h");
    ctd_string replace_3 = ctd_string_create_from_literal("");
    ctd_string compare_1 = ctd_string_create_from_literal("Hi hhhh! hhhh is a thing over hhhh");
    ctd_string compare_2 = ctd_string_create_from_literal("Hi there! h is a thing over h");
    ctd_string compare_3 = ctd_string_create_from_literal("H hhhh! hhhh s a thng over hhhh");

    ctd_string_builder_append(&builder_1, str, &error);
    if (error.error_type != NO_ERROR) goto cleanup;
    ctd_string_builder_append(&builder_2, str, &error);
    if (error.error_type != NO_ERROR) goto cleanup;

    ctd_string_builder_replace_all(&builder_1, search_1, replace_1, 0, &error);
    if (error.error_type != NO_ERROR || !ctd_string_equals(ctd_string_builder_to_span(&builder_1, 0, builder_1.length, &error), compare_1)) goto cleanup;

    ctd_string_builder_replace_all(&builder_2, search_1, replace_2, 4, &error);
    if (error.error_type != NO_ERROR || !ctd_string_equals(ctd_string_builder_to_span(&builder_2, 0, builder_2.length, &error), compare_2)) goto cleanup;

    ctd_string_builder_replace_all(&builder_1, search_2, replace_3, 0, &error);
    if (error.error_type != NO_ERROR || !ctd_string_equals(ctd_string_builder_to_span(&builder_1, 0, builder_1.length, &error), compare_3)) goto cleanup;

    ctd_string_builder_destroy(&builder_1);
    ctd_string_builder_destroy(&builder_2);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder_1);
    ctd_string_builder_destroy(&builder_2);
    return 1;
}

static int test_ctd_string_builder_reverse()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    ctd_string str_1 = ctd_string_create_from_literal("Hi there!");
    ctd_string str_2 = ctd_string_create_from_literal("!ereht iH");

    ctd_string_builder_append(&builder, str_1, &error);
    ctd_string_builder_reverse(&builder);
    ctd_string builder_string = {.data = builder.data, .length = builder.length};
    if (!ctd_string_equals(builder_string, str_2) || error.error_type != NO_ERROR)
    {
        goto cleanup;
    }

    ctd_string_builder_destroy(&builder);
    return 0;

cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

static int test_ctd_string_builder_clear()
{
    Error error = {0};
    ctd_allocator allocator = ctd_default_allocator_create().allocator;
    ctd_string_builder builder = ctd_string_builder_create(100 * sizeof(char), &allocator, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    ctd_string str_1 = ctd_string_create_from_literal("Hi there!");
    ctd_string_builder_append(&builder, str_1, &error);
    if (error.error_type != NO_ERROR)
    {
        goto cleanup;
    }
    if (builder.length == 0)
    {
        goto cleanup;
    }
    ctd_string_builder_clear(&builder);
    if (builder.length != 0)
    {
        goto cleanup;
    }

    ctd_string_builder_destroy(&builder);
    return 0;
cleanup:
    ctd_string_builder_destroy(&builder);
    return 1;
}

void test_ctd_string_methods()
{
    int status;
    uint32_t number_of_tests_failed = 0;
    printf("---------- Begin ctd_string Test ----------\n");
    RUN_TEST(ctd_string_create_from_literal, status, number_of_tests_failed)
    RUN_TEST(ctd_string_span, status, number_of_tests_failed)
    RUN_TEST(ctd_string_equals, status, number_of_tests_failed)
    RUN_TEST(ctd_string_compare, status, number_of_tests_failed)
    RUN_TEST(ctd_string_find, status, number_of_tests_failed)
    RUN_TEST(ctd_string_reverse_find, status, number_of_tests_failed)
    RUN_TEST(ctd_string_remove_whitespace, status, number_of_tests_failed)
    RUN_TEST(ctd_string_copy, status, number_of_tests_failed)
    RUN_TEST(ctd_string_to_c_string, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_push_back, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_pop_back, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_append, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_insert, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_remove, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_find, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_contains, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_replace, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_replace_all, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_reverse, status, number_of_tests_failed)
    RUN_TEST(ctd_string_builder_clear, status, number_of_tests_failed)

    if (number_of_tests_failed == 0)
    {
        printf("\x1b[32mAll tests passed!\x1b[0m\n");
    }
    else
    {
        printf("\x1b[31m%u tests failed.\x1b[0m\n", number_of_tests_failed);
    }
    printf("---------- End ctd_string Test ----------\n");
}
