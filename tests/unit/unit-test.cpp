#include <gtest/gtest.h>

extern "C" {
#include "../../src/calcstack.h"
}

// Initialize

TEST(InitializationTest, OperabilityInt)
{
    calc_stack test;
    initialize(&test, 0);
    EXPECT_EQ(test.int_stack.top, -1);
    EXPECT_EQ(test.char_stack.top, -1);
}

TEST(InitializationTest, OperabilityFloat)
{
    calc_stack test;
    initialize(&test, 1);
    EXPECT_EQ(test.float_stack.top, -1);
    EXPECT_EQ(test.char_stack.top, -1);
}

// IsEmpty

TEST(IsEmptyTest, IntTrue)
{
    calc_stack test;
    initialize(&test, 0);
    EXPECT_TRUE(isEmptyInt(&test.int_stack));
}

TEST(IsEmptyTest, IntFalse)
{
    calc_stack test;
    initialize(&test, 0);
    test.int_stack.top++;
    EXPECT_FALSE(isEmptyInt(&test.int_stack));
}

TEST(IsEmptyTest, FloatTrue)
{
    calc_stack test;
    initialize(&test, 1);
    EXPECT_TRUE(isEmptyFloat(&test.float_stack));
}

TEST(IsEmptyTest, FloatFalse)
{
    calc_stack test;
    initialize(&test, 1);
    test.float_stack.top++;
    EXPECT_FALSE(isEmptyFloat(&test.float_stack));
}

TEST(IsEmptyTest, CharTrue)
{
    calc_stack test;
    initialize(&test, 0);
    EXPECT_TRUE(isEmptyChar(&test.char_stack));
}

TEST(IsEmptyTest, CharFalse)
{
    calc_stack test;
    initialize(&test, 0);
    test.char_stack.top++;
    EXPECT_FALSE(isEmptyChar(&test.char_stack));
}

// IsFull

TEST(IsFullTest, IntTrue)
{
    calc_stack test;
    initialize(&test, 0);
    test.int_stack.top = 511;
    EXPECT_TRUE(isFullInt(&test.int_stack));
}

TEST(IsFullTest, IntFalse)
{
    calc_stack test;
    initialize(&test, 0);
    EXPECT_FALSE(isFullInt(&test.int_stack));
}

TEST(IsFullTest, FloatTrue)
{
    calc_stack test;
    initialize(&test, 1);
    test.float_stack.top = 511;
    EXPECT_TRUE(isFullFloat(&test.float_stack));
}

TEST(IsFullTest, FloatFalse)
{
    calc_stack test;
    initialize(&test, 1);
    EXPECT_FALSE(isFullFloat(&test.float_stack));
}

TEST(IsFullTest, CharTrue)
{
    calc_stack test;
    initialize(&test, 0);
    test.char_stack.top = 1023;
    EXPECT_TRUE(isFullChar(&test.char_stack));
}

TEST(IsFullTest, CharFalse)
{
    calc_stack test;
    initialize(&test, 0);
    EXPECT_FALSE(isFullChar(&test.char_stack));
}

// Push

TEST(Push, IntTrue)
{
    calc_stack test;
    initialize(&test, 0);
    long number = 7;
    push_int(&test.int_stack, number);
    EXPECT_EQ(test.int_stack.arr[test.int_stack.top], number);
    EXPECT_TRUE(!isEmptyInt(&test.int_stack));
}

TEST(Push, IntFalse)
{
    calc_stack test;
    initialize(&test, 0);
    test.int_stack.top = 511;
    long number = 4;
    EXPECT_EXIT(push_int(&test.int_stack, number), ::testing::ExitedWithCode(7),
        "");
}

TEST(Push, FloatTrue)
{
    calc_stack test;
    initialize(&test, 1);
    float number = 1.5;
    push_float(&test.float_stack, number);
    EXPECT_EQ(test.float_stack.arr[test.float_stack.top], number);
    EXPECT_TRUE(!isEmptyFloat(&test.float_stack));
}

TEST(Push, FloatFalse)
{
    calc_stack test;
    initialize(&test, 1);
    test.float_stack.top = 511;
    float number = 6.8;
    EXPECT_EXIT(push_float(&test.float_stack, number),
        ::testing::ExitedWithCode(7), "");
}

TEST(Push, CharTrue)
{
    calc_stack test;
    initialize(&test, 0);
    char symb = 'C';
    push_char(&test.char_stack, symb);
    EXPECT_EQ(test.char_stack.arr[test.char_stack.top], symb);
    EXPECT_TRUE(!isEmptyChar(&test.char_stack));
}

TEST(Push, CharFalse)
{
    calc_stack test;
    initialize(&test, 0);
    test.char_stack.top = 1023;
    char symb = 'C';
    EXPECT_EXIT(push_char(&test.char_stack, symb), ::testing::ExitedWithCode(7),
        "");
}

// Pop

TEST(Pop, IntTrue)
{
    calc_stack test;
    initialize(&test, 0);
    long number = 7;
    push_int(&test.int_stack, number);
    long check = pop_int(&test.int_stack);
    EXPECT_EQ(check, number);
    EXPECT_TRUE(isEmptyInt(&test.int_stack));
}

TEST(Pop, IntFalse)
{
    calc_stack test;
    initialize(&test, 0);
    EXPECT_EXIT(pop_int(&test.int_stack), ::testing::ExitedWithCode(8), "");
}

TEST(Pop, FloatTrue)
{
    calc_stack test;
    initialize(&test, 1);
    float number = 1.5;
    push_float(&test.float_stack, number);
    float check = pop_float(&test.float_stack);
    EXPECT_EQ(check, number);
    EXPECT_TRUE(isEmptyFloat(&test.float_stack));
}

TEST(Pop, FloatFalse)
{
    calc_stack test;
    initialize(&test, 1);
    EXPECT_EXIT(pop_float(&test.float_stack), ::testing::ExitedWithCode(8), "");
}

TEST(Pop, CharTrue)
{
    calc_stack test;
    initialize(&test, 0);
    char symb = 'C';
    push_char(&test.char_stack, symb);
    char check = pop_char(&test.char_stack);
    EXPECT_EQ(check, symb);
    EXPECT_TRUE(isEmptyChar(&test.char_stack));
}

TEST(Pop, CharFalse)
{
    calc_stack test;
    initialize(&test, 0);
    EXPECT_EXIT(pop_char(&test.char_stack), ::testing::ExitedWithCode(8), "");
}

// Peek

TEST(Peek, CharTrue)
{
    calc_stack test;
    initialize(&test, 0);
    char symb = 'C';
    push_char(&test.char_stack, symb);
    char check = peek_char(&test.char_stack);
    EXPECT_EQ(check, symb);
    EXPECT_TRUE(!isEmptyChar(&test.char_stack));
}

TEST(Peek, CharFalse)
{
    calc_stack test;
    initialize(&test, 0);
    EXPECT_EXIT(peek_char(&test.char_stack), ::testing::ExitedWithCode(8), "");
}