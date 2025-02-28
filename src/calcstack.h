#pragma once
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 512

// В идеале сделать расширение массивов, в случае переполнения, но пока можно
// оставить так
typedef struct {
    long arr[BUF_SIZE];
    int top;
} stack_of_numbers_int;

typedef struct {
    float arr[BUF_SIZE];
    int top;
} stack_of_numbers_float;

typedef struct {
    char arr[2 * BUF_SIZE];
    int top;
} stack_of_operations;

typedef struct {
    stack_of_numbers_int int_stack;
    stack_of_numbers_float float_stack;
    stack_of_operations char_stack;
} calc_stack;

void initialize(calc_stack* stack, bool float_flag);

bool isEmptyInt(stack_of_numbers_int* int_stack);

bool isEmptyFloat(stack_of_numbers_float* float_stack);

bool isEmptyChar(stack_of_operations* char_stack);

bool isFullInt(stack_of_numbers_int* int_stack);

bool isFullFloat(stack_of_numbers_float* float_stack);

bool isFullChar(stack_of_operations* char_stack);

void push_int(stack_of_numbers_int* int_stack, long value);

void push_float(stack_of_numbers_float* float_stack, float value);

void push_char(stack_of_operations* char_stack, char value);

long pop_int(stack_of_numbers_int* int_stack);

float pop_float(stack_of_numbers_float* float_stack);

char pop_char(stack_of_operations* char_stack);

char peek_char(stack_of_operations* char_stack);