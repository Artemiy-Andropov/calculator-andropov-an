#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#pragma warning(disable : 4996)

int main()
{
    // В идеале сделать расширение массивов, в случае переполнения, но пока можно оставить так
    int stack_of_numbers[128];
    stack_of_numbers[0] = 0;
    int st_num_pos = 1;
    char stack_of_operations[128];
    int st_oper_pos = 0;

    char str[256];
    scanf("%256[^\n]", str);
    size_t len_str = strlen(str);

    bool prev_num = 0;
    int number = 0;
    bool less_zero = 0;
    size_t less_zero_bracket = 1;

    for (int i = 0; i < len_str; i++)
    {
        if (isdigit(str[i]))
        {
            if (prev_num == 0)
            {
                number = ((int)str[i] - '0');
                prev_num = 1;
            }
            else
                number = number * 10 + ((int)str[i] - '0');
        }
        else if (isspace(str[i]))
        {
            if (prev_num == 0)
                continue;
            else
            {
                if (less_zero)
                {
                    stack_of_numbers[st_num_pos] = -number;
                    less_zero = 0;
                }
                else
                    stack_of_numbers[st_num_pos] = number;
                st_num_pos++;
                prev_num = 0;
            }
        }
        else
        {
            if (prev_num != 0)
            {
                if (less_zero)
                {
                    stack_of_numbers[st_num_pos] = -number;
                    less_zero = 0;
                }
                else
                    stack_of_numbers[st_num_pos] = number;
                st_num_pos++;
                prev_num = 0;
            }

            if (str[i] == '-')
            {
                less_zero = 1;
                str[i] = '+';
            }

            if (st_oper_pos == 0)
            {
                stack_of_operations[0] = str[i];
                st_oper_pos++;
            }
            else
            {
                switch (str[i])
                {
                case ('+'):
                    if (stack_of_operations[st_oper_pos - 1] == '+')
                    {
                        stack_of_numbers[st_num_pos - 2] += stack_of_numbers[st_num_pos - 1];
                        st_num_pos--;
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '*')
                    {
                        stack_of_numbers[st_num_pos - 2] *= stack_of_numbers[st_num_pos - 1];
                        st_num_pos--;
                        stack_of_operations[st_oper_pos - 1] = '+';
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '/')
                    {
                        stack_of_numbers[st_num_pos - 2] /= stack_of_numbers[st_num_pos - 1];
                        st_num_pos--;
                        stack_of_operations[st_oper_pos - 1] = '+';
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '(')
                    {
                        stack_of_operations[st_oper_pos] = '+';
                        st_oper_pos++;
                    }
                    break;
                case ('*'):
                    if (stack_of_operations[st_oper_pos - 1] == '+')
                    {
                        stack_of_operations[st_oper_pos] = '*';
                        st_oper_pos++;
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '*')
                    {
                        stack_of_numbers[st_num_pos - 2] *= stack_of_numbers[st_num_pos - 1];
                        st_num_pos--;
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '/')
                    {
                        stack_of_numbers[st_num_pos - 2] /= stack_of_numbers[st_num_pos - 1];
                        st_num_pos--;
                        stack_of_operations[st_oper_pos - 1] = '*';
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '(')
                    {
                        stack_of_operations[st_oper_pos] = '*';
                        st_oper_pos++;
                    }
                    break;
                case ('/'):
                    if (stack_of_operations[st_oper_pos - 1] == '+')
                    {
                        stack_of_operations[st_oper_pos] = '/';
                        st_oper_pos++;
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '*')
                    {
                        stack_of_numbers[st_num_pos - 2] *= stack_of_numbers[st_num_pos - 1];
                        st_num_pos--;
                        stack_of_operations[st_oper_pos - 1] = '/';
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '/')
                    {
                        stack_of_numbers[st_num_pos - 2] /= stack_of_numbers[st_num_pos - 1];
                        st_num_pos--;
                    }
                    else if (stack_of_operations[st_oper_pos - 1] == '(')
                    {
                        stack_of_operations[st_oper_pos] = '/';
                        st_oper_pos++;
                    }
                    break;
                case ('('):
                    // Рассматриваем ситуацию, когда минус стоит перед скобкой
                    if (less_zero)
                    {
                        stack_of_operations[st_oper_pos] = '*';
                        st_oper_pos++;
                        stack_of_numbers[st_num_pos] = -1;
                        st_num_pos++;
                        less_zero = 0;
                    }
                    // Предпологаем, что ввод правильный :)
                    stack_of_operations[st_oper_pos] = '(';
                    st_oper_pos++;
                    break;
                case (')'):
                    while (stack_of_operations[st_oper_pos - 1] != '(')
                    {
                        if (stack_of_operations[st_oper_pos - 1] == '+')
                            stack_of_numbers[st_num_pos - 2] += stack_of_numbers[st_num_pos - 1];

                        if (stack_of_operations[st_oper_pos - 1] == '*')
                            stack_of_numbers[st_num_pos - 2] *= stack_of_numbers[st_num_pos - 1];

                        if (stack_of_operations[st_oper_pos - 1] == '/')
                            stack_of_numbers[st_num_pos - 2] /= stack_of_numbers[st_num_pos - 1];

                        st_num_pos--;
                        st_oper_pos--;
                    }
                    st_oper_pos--;

                    break;
                default:
                    printf("Error. Incorrect input.");
                    break;
                }
            }
        }
    }

    if (prev_num != 0)
    {
        stack_of_numbers[st_num_pos] = number;
        st_num_pos++;
        prev_num = 0;
    }

    while (st_oper_pos != 0)
    {
        if (stack_of_operations[st_oper_pos - 1] == '+')
            stack_of_numbers[st_num_pos - 2] += stack_of_numbers[st_num_pos - 1];

        if (stack_of_operations[st_oper_pos - 1] == '*')
            stack_of_numbers[st_num_pos - 2] *= stack_of_numbers[st_num_pos - 1];

        if (stack_of_operations[st_oper_pos - 1] == '/')
            stack_of_numbers[st_num_pos - 2] /= stack_of_numbers[st_num_pos - 1];

        st_num_pos--;
        st_oper_pos--;
    }

    if (st_num_pos == 2)
    {
        stack_of_numbers[st_num_pos - 2] += stack_of_numbers[st_num_pos - 1];
        st_num_pos--;
    }

    printf("%d", stack_of_numbers[0]);

    return 0;
}