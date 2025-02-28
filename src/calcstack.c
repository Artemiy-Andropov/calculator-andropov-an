#include "calcstack.h"

void initialize(calc_stack* stack, bool float_flag)
{
    if (float_flag)
        stack->float_stack.top = -1;
    else
        stack->int_stack.top = -1;
    stack->char_stack.top = -1;
}

bool isEmptyInt(stack_of_numbers_int* int_stack)
{
    return int_stack->top == -1;
}

bool isEmptyFloat(stack_of_numbers_float* float_stack)
{
    return float_stack->top == -1;
}

bool isEmptyChar(stack_of_operations* char_stack)
{
    return char_stack->top == -1;
}

bool isFullInt(stack_of_numbers_int* int_stack)
{
    return int_stack->top == BUF_SIZE - 1;
}

bool isFullFloat(stack_of_numbers_float* float_stack)
{
    return float_stack->top == BUF_SIZE - 1;
}

bool isFullChar(stack_of_operations* char_stack)
{
    return char_stack->top == 2 * BUF_SIZE - 1;
}

// может переделать void на int, чтобы отслеживать ошибки?
void push_int(stack_of_numbers_int* int_stack, long value)
{
    if (isFullInt(int_stack))
        exit(7); // Stack Overflow
    int_stack->top++;
    int_stack->arr[int_stack->top] = value;
}

void push_float(stack_of_numbers_float* float_stack, float value)
{
    if (isFullFloat(float_stack))
        exit(7); // Stack Overflow
    float_stack->top++;
    float_stack->arr[float_stack->top] = value;
}

void push_char(stack_of_operations* char_stack, char value)
{
    if (isFullChar(char_stack))
        exit(7); // Stack Overflow
    char_stack->top++;
    char_stack->arr[char_stack->top] = value;
}

long pop_int(stack_of_numbers_int* int_stack)
{
    if (isEmptyInt(int_stack))
        exit(8); // Stack Underflow
    long value = int_stack->arr[int_stack->top];
    int_stack->top--;
    return value;
}

float pop_float(stack_of_numbers_float* float_stack)
{
    if (isEmptyFloat(float_stack))
        exit(8); // Stack Underflow
    float value = float_stack->arr[float_stack->top];
    float_stack->top--;
    return value;
}

char pop_char(stack_of_operations* char_stack)
{
    if (isEmptyChar(char_stack))
        exit(8); // Stack Underflow
    char value = char_stack->arr[char_stack->top];
    char_stack->top--;
    return value;
}

char peek_char(stack_of_operations* char_stack)
{
    if (isEmptyChar(char_stack))
        exit(8); // Stack Underflow
    return char_stack->arr[char_stack->top];
}