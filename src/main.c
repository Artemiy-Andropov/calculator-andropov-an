#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#pragma warning(disable : 4996)
//#pragma warning(disable : 5045)

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

#ifndef GTEST
int main(int argc, char* argv[])
{
    bool float_flag = false;
    if (strcmp(argv[argc - 1], "--float") == 0)
        float_flag = true;

    /*
    if (float_flag)
        printf("The --float flag has been passed\n");
    else
        printf("The --float flag was not passed\n");
    */

    calc_stack stack;
    initialize(&stack, float_flag);

    /*
    * In case of a unary "-"
    if (float_flag)
        push_float(&stack.float_stack, 0.0);
    else
        push_float(&stack.int_stack, 0);
    */

    const char* target_oper = "+-*/()";

    char str[1025];
    scanf("%1025[^\n]", str);
    int len_str = (int)strlen(str);

    if (len_str > 1024)
        return 1; // Exceeding the input

    bool prev_num = 0;
    bool prev_oper = 0;
    bool less_zero = 0;
    bool start = 1;

    short correct_placement_parentheses = 0;
    long number = 0;
    char symb;

    for (int i = 0; i < len_str; i++) {
        symb = str[i];
        if (isdigit(symb)) {
            if (prev_oper == 0 && start == 0 && prev_num == 0)
                return 2; // Incorrect input of an arithmetic expression. Numbers
                          // without an operation between them.

            if (prev_num == 0) {
                number = ((int)symb - '0');
                prev_num = 1;
                start = 0;
                prev_oper = 0;
            } else {
                if ((double)(number * 10 + ((int)symb - '0')) < (2 * pow(10, 9)))
                    number = number * 10 + ((int)symb - '0');
                else
                    return 3; // Exceeding the entered values
            }
        } else if (isspace(symb)) {
            if (prev_num == 0)
                continue;
            else {
                if (less_zero) {
                    if (float_flag)
                        push_float(&stack.float_stack, -(float)number);
                    else
                        push_int(&stack.int_stack, -number);
                    less_zero = 0;
                } else {
                    if (float_flag)
                        push_float(&stack.float_stack, (float)number);
                    else
                        push_int(&stack.int_stack, number);
                }
                prev_num = 0;
            }
        } else {
            if (strchr(target_oper, symb) == NULL)
                return 4; // Incorrect input of an arithmetic expression. Unknown
                          // operations.

            if (prev_oper && symb != '(')
                return 9; // Incorrect input of an arithmetic expression. Two operations
                          // in a row.
            else
                prev_oper = 1;

            if (prev_num) {
                if (less_zero) {
                    if (float_flag)
                        push_float(&stack.float_stack, -(float)number);
                    else
                        push_int(&stack.int_stack, -number);
                    less_zero = 0;
                } else {
                    if (float_flag)
                        push_float(&stack.float_stack, (float)number);
                    else
                        push_int(&stack.int_stack, number);
                }
                prev_num = 0;
            }

            if (symb == '-') {
                less_zero = 1;
                symb = '+';
            }

            if (isEmptyChar(&stack.char_stack)) {
                if (symb == '(')
                    correct_placement_parentheses++;
                else if (symb == ')')
                    return 6; // Incorrect input of an arithmetic expression. Incorrect
                              // placement of parentheses.
                push_char(&stack.char_stack, symb);
            } else {
                char symb_prev_oper = peek_char(&stack.char_stack);
                switch (symb) {
                case ('+'):
                    switch (symb_prev_oper) {
                    case ('+'):
                        if (float_flag) {
                            stack.float_stack.arr[stack.float_stack.top - 1] += stack.float_stack.arr[stack.float_stack.top];
                            pop_float(&stack.float_stack);
                        } else {
                            stack.int_stack.arr[stack.int_stack.top - 1] += stack.int_stack.arr[stack.int_stack.top];
                            pop_int(&stack.int_stack);
                        }
                        break;
                    case ('*'):
                        if (float_flag) {
                            stack.float_stack.arr[stack.float_stack.top - 1] *= stack.float_stack.arr[stack.float_stack.top];
                            pop_float(&stack.float_stack);
                        } else {
                            stack.int_stack.arr[stack.int_stack.top - 1] *= stack.int_stack.arr[stack.int_stack.top];
                            pop_int(&stack.int_stack);
                        }
                        stack.char_stack.arr[stack.char_stack.top] = symb;
                        break;
                    case ('/'):
                        if (float_flag) {
                            float denominator = pop_float(&stack.float_stack);
                            if (fabs(denominator) < pow(10, -4))
                                return 5; // Incorrect arithmetic expression. Dividing by a
                                          // number close to zero.

                            stack.float_stack.arr[stack.float_stack.top] /= denominator;
                        } else {
                            long denominator = pop_int(&stack.int_stack);
                            if (denominator == 0)
                                return 5; // Incorrect arithmetic expression. Dividing by a
                                          // number close to zero. :)

                            stack.int_stack.arr[stack.int_stack.top] /= denominator;
                        }
                        stack.char_stack.arr[stack.char_stack.top] = symb;
                        break;
                    case ('('):
                        push_char(&stack.char_stack, symb);
                        break;
                    default:
                        return 4; // Incorrect input of an arithmetic expression. Unknown
                                  // operations.
                    }
                    break;
                case ('*'):
                    switch (symb_prev_oper) {
                    case ('+'):
                        push_char(&stack.char_stack, symb);
                        break;
                    case ('*'):
                        if (float_flag) {
                            stack.float_stack.arr[stack.float_stack.top - 1] *= stack.float_stack.arr[stack.float_stack.top];
                            pop_float(&stack.float_stack);
                        } else {
                            stack.int_stack.arr[stack.int_stack.top - 1] *= stack.int_stack.arr[stack.int_stack.top];
                            pop_int(&stack.int_stack);
                        }
                        break;
                    case ('/'):
                        if (float_flag) {
                            float denominator = pop_float(&stack.float_stack);
                            if (fabs(denominator) < pow(10, -4))
                                return 5; // Incorrect arithmetic expression. Dividing by a
                                          // number close to zero.

                            stack.float_stack.arr[stack.float_stack.top] /= denominator;
                        } else {
                            long denominator = pop_int(&stack.int_stack);
                            if (denominator == 0)
                                return 5; // Incorrect arithmetic expression. Dividing by a
                                          // number close to zero. :)

                            stack.int_stack.arr[stack.int_stack.top] /= denominator;
                        }
                        stack.char_stack.arr[stack.char_stack.top] = symb;
                        break;
                    case ('('):
                        push_char(&stack.char_stack, symb);
                        break;
                    default:
                        return 4; // Incorrect input of an arithmetic expression. Unknown
                                  // operations.
                    }
                    break;
                case ('/'):
                    switch (symb_prev_oper) {
                    case ('+'):
                        push_char(&stack.char_stack, symb);
                        break;
                    case ('*'):
                        if (float_flag) {
                            stack.float_stack.arr[stack.float_stack.top - 1] *= stack.float_stack.arr[stack.float_stack.top];
                            pop_float(&stack.float_stack);
                        } else {
                            stack.int_stack.arr[stack.int_stack.top - 1] *= stack.int_stack.arr[stack.int_stack.top];
                            pop_int(&stack.int_stack);
                        }
                        stack.char_stack.arr[stack.char_stack.top] = symb;
                        break;
                    case ('/'):
                        if (float_flag) {
                            float denominator = pop_float(&stack.float_stack);
                            if (fabs(denominator) < pow(10, -4))
                                return 5; // Incorrect arithmetic expression. Dividing by a
                                          // number close to zero.

                            stack.float_stack.arr[stack.float_stack.top] /= denominator;
                        } else {
                            long denominator = pop_int(&stack.int_stack);
                            if (denominator == 0)
                                return 5; // Incorrect arithmetic expression. Dividing by a
                                          // number close to zero. :)

                            stack.int_stack.arr[stack.int_stack.top] /= denominator;
                        }
                        break;
                    case ('('):
                        push_char(&stack.char_stack, symb);
                        break;
                    default:
                        return 4; // Incorrect input of an arithmetic expression. Unknown
                                  // operations.
                    }
                    break;
                case ('('):
                    // Рассматриваем ситуацию, когда минус стоит перед скобкой
                    if (less_zero) {
                        push_char(&stack.char_stack, '*');
                        if (float_flag)
                            push_float(&stack.float_stack, -1);
                        else
                            push_int(&stack.int_stack, -1);
                        less_zero = 0;
                    }
                    push_char(&stack.char_stack, symb);
                    correct_placement_parentheses++;
                    break;
                case (')'):
                    if (correct_placement_parentheses == 0)
                        return 6; // Incorrect input of an arithmetic expression. Incorrect
                                  // placement of parentheses.

                    prev_oper = 0;
                    correct_placement_parentheses--;
                    symb_prev_oper = pop_char(&stack.char_stack);
                    while (symb_prev_oper != '(') {
                        switch (symb_prev_oper) {
                        case ('+'):
                            if (float_flag) {
                                stack.float_stack.arr[stack.float_stack.top - 1] += stack.float_stack.arr[stack.float_stack.top];
                                pop_float(&stack.float_stack);
                            } else {
                                stack.int_stack.arr[stack.int_stack.top - 1] += stack.int_stack.arr[stack.int_stack.top];
                                pop_int(&stack.int_stack);
                            }
                            break;
                        case ('*'):
                            if (float_flag) {
                                stack.float_stack.arr[stack.float_stack.top - 1] *= stack.float_stack.arr[stack.float_stack.top];
                                pop_float(&stack.float_stack);
                            } else {
                                stack.int_stack.arr[stack.int_stack.top - 1] *= stack.int_stack.arr[stack.int_stack.top];
                                pop_int(&stack.int_stack);
                            }
                            break;
                        case ('/'):
                            if (float_flag) {
                                float denominator = pop_float(&stack.float_stack);
                                if (fabs(denominator) < pow(10, -4))
                                    return 5; // Incorrect arithmetic expression. Dividing by a
                                              // number close to zero.

                                stack.float_stack.arr[stack.float_stack.top] /= denominator;
                            } else {
                                long denominator = pop_int(&stack.int_stack);
                                if (denominator == 0)
                                    return 5; // Incorrect arithmetic expression. Dividing by a
                                              // number close to zero. :)

                                stack.int_stack.arr[stack.int_stack.top] /= denominator;
                            }
                            break;
                        default:
                            return 4; // Incorrect input of an arithmetic expression. Unknown
                                      // operations.
                        }

                        symb_prev_oper = pop_char(&stack.char_stack);
                    }
                    break;
                default:
                    return 4; // Incorrect input of an arithmetic expression. Unknown
                              // operations.
                }
            }
        }
    }

    if (prev_num) {
        if (less_zero) {
            if (float_flag)
                push_float(&stack.float_stack, -(float)number);
            else
                push_int(&stack.int_stack, -number);
            less_zero = 0;
        } else {
            if (float_flag)
                push_float(&stack.float_stack, (float)number);
            else
                push_int(&stack.int_stack, number);
        }
        prev_num = 0;
    }

    while (!isEmptyChar(&stack.char_stack)) {
        char symb_oper = pop_char(&stack.char_stack);
        switch (symb_oper) {
        case ('+'):
            if (float_flag) {
                stack.float_stack.arr[stack.float_stack.top - 1] += stack.float_stack.arr[stack.float_stack.top];
                pop_float(&stack.float_stack);
            } else {
                stack.int_stack.arr[stack.int_stack.top - 1] += stack.int_stack.arr[stack.int_stack.top];
                pop_int(&stack.int_stack);
            }
            break;
        case ('*'):
            if (float_flag) {
                stack.float_stack.arr[stack.float_stack.top - 1] *= stack.float_stack.arr[stack.float_stack.top];
                pop_float(&stack.float_stack);
            } else {
                stack.int_stack.arr[stack.int_stack.top - 1] *= stack.int_stack.arr[stack.int_stack.top];
                pop_int(&stack.int_stack);
            }
            break;
        case ('/'):
            if (float_flag) {
                float denominator = pop_float(&stack.float_stack);
                if (fabs(denominator) < pow(10, -4))
                    return 5; // Incorrect arithmetic expression. Dividing by a number
                              // close to zero.

                stack.float_stack.arr[stack.float_stack.top] /= denominator;
            } else {
                long denominator = pop_int(&stack.int_stack);
                if (denominator == 0)
                    return 5; // Incorrect arithmetic expression. Dividing by a number
                              // close to zero. :)

                stack.int_stack.arr[stack.int_stack.top] /= denominator;
            }
            break;
        default:
            return 4; // Incorrect input of an arithmetic expression. Unknown
                      // operations.
        }
    }

    /*
    * In case of a unary "-"
    if (float_flag)
    {
        if (stack.float_stack.arr[stack.float_stack.top] == 1)
        {
            stack.float_stack.arr[stack.float_stack.top - 1] +=
    stack.float_stack.arr[stack.float_stack.top]; pop_float(&stack.float_stack);
        }
    }
    else
    {
        if (stack.int_stack.arr[stack.int_stack.top] == 1)
        {
            stack.int_stack.arr[stack.int_stack.top - 1] +=
    stack.int_stack.arr[stack.int_stack.top]; pop_int(&stack.int_stack);
        }
    }
    */

    if (float_flag)
        printf("%.4f", pop_float(&stack.float_stack));
    else
        printf("%ld", pop_int(&stack.int_stack));

    return 0;
}
#endif