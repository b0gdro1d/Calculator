#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SMAX 100

int find_id (const char mas_id[SMAX][SMAX], int mas_len, const char* word)
{
    for (int i = 0; i < mas_len; i+=2)
    {
        if ((strncmp(word, mas_id[i], strlen(word)) == 0)) return i;
    }
    return -1;
}

char type_of_operand (const char* number)
{
    for (int m = 1; m < 100; ++m)
    {
        if (number[m] == '.') return 'd';
        if (number[m] == '\0') break;
    }
    return 'i';
}

int add_to_mas(char mas_id[][SMAX], int mas_len, double* mas_result, const char* word, double result, char type)
{
    mas_len += 2;
    strncpy(mas_id[mas_len-2], word, SMAX);
    if (type == 'i')
    {
        mas_id[mas_len-1][0] = 'i';
        mas_result[mas_len-2] = (int) result;
    }
    else
    {
        mas_id[mas_len-1][0] = 'd';
        mas_result[mas_len-2] = result;
    }
    return mas_len;
}

int calc (char mas_id[SMAX][SMAX], double* mas_result, char** mas_inp, int start, int end, int mas_id_len)
{
    int i = start, j = end;
    int k = j;
    int l,n, i1, k_p, k_l;
    bool t_a = 0, t_b = 0;
    char type_a, type_b;
    for (; i < j; ++i)
    {
        if (mas_inp[i][0] == '(')
        {
            mas_inp[i] = " ";
            while (mas_inp[k][0] != ')') --k;
            mas_inp[k] = " ";
            i1 = i + 1;
            calc(mas_id, mas_result, mas_inp, i1, k, mas_id_len);
        }
        if ((mas_inp[i][0] == '*') || (mas_inp[i][0] == '/') || (mas_inp[i][0] == '%'))
        {
            char c = mas_inp[i][0];
            k = i - 1;
            l = i + 1;
            while (mas_inp[k][0] == ' ') --k;
            while (mas_inp[l][0] == ' ') ++l;
            if (((mas_inp[k][0] == '-') || (mas_inp[k][0] >= '0' && mas_inp[k][0] <= '9')))
            {
                type_a = type_of_operand(mas_inp[k]);
                t_a = 1;
            }
            else
            {
                k_p = find_id(mas_id, mas_id_len, mas_inp[k]);
                if (mas_id[k_p+1][0] == 'i') type_a = 'i';
                else type_a = 'd';
            }

            if ((((mas_inp[l][0] == '-') && (mas_inp[l][1] >= '0' && mas_inp[l][1] <= '9') ) || (mas_inp[l][0] >= '0' && mas_inp[l][0] <= '9')))
            {
                type_b = type_of_operand(mas_inp[l]);
                t_b = 1;
            }
            else
            {
                k_l = find_id(mas_id, mas_id_len, mas_inp[k]);
                if (mas_id[k_l+1][0] == 'i') type_b = 'i';
                else type_b = 'd';
            }

            switch (c)
            {
                case ('*'):
                    if ((type_a == 'i') && (type_b == 'i'))
                    {
                        int result = 1;
                        if (t_a) result *= atoi(mas_inp[k]);
                        else result *= (int)mas_result[k_p];
                        if (t_b) result *= atoi(mas_inp[l]);
                        else result *= (int)mas_result[k_l];
                        sprintf(mas_inp[i], "%d", result);;
                    }
                    else
                    {
                        double result = 1.0;
                        if (t_a) result *= atof(mas_inp[k]);
                        else result *= mas_result[k_p];
                        if (t_b) result *= atof(mas_inp[l]);
                        else result *= mas_result[k_l];
                        sprintf(mas_inp[i], "%f", result);
                    }
                    break;
                case ('/'):
                    if ((type_a == 'i') && (type_b == 'i'))
                    {
                        int result = 1;
                        if (t_a) result *= atoi(mas_inp[k]);
                        else result *= (int)mas_result[k_p];
                        if (t_b) result /= atoi(mas_inp[l]);
                        else result /= (int)mas_result[k_l];
                        sprintf(mas_inp[i], "%d", result);
                    }
                    else
                    {
                        double result = 1.0;
                        if (t_a) result *= atof(mas_inp[k]);
                        else result *= mas_result[k_p];
                        if (t_b) result /= atof(mas_inp[l]);
                        else result /= mas_result[k_l];
                        sprintf(mas_inp[i], "%f", result);
                    }
                    break;
                case ('%'):
                {
                    int result = 1;
                    if (t_a) result *= atoi(mas_inp[k]);
                    else result *= (int)mas_result[k_p];
                    if (t_b) result %= atoi(mas_inp[l]);
                    else result %= (int)mas_result[k_l];
                    sprintf(mas_inp[i], "%d", result);
                }
            }
            mas_inp[k] = mas_inp[l] = " ";
        }
    }
    i = start;
    for (; i < j; ++i)
    {
        if ((mas_inp[i][0] == '+') || ((mas_inp[i][0] == '-') && (mas_inp[i][1] =='\0') ))
        {
            char c = mas_inp[i][0];
            k = i - 1;
            l = i + 1;
            while (mas_inp[k][0] == ' ') --k;
            while (mas_inp[l][0] == ' ') ++l;
            if (((mas_inp[k][0] == '-') || (mas_inp[k][0] >= '0' && mas_inp[k][0] <= '9')))
            {
                type_a = type_of_operand(mas_inp[k]);
                t_a = 1;
            }
            else
            {
                k_p = find_id(mas_id, mas_id_len, mas_inp[k]);
                if (mas_id[k_p+1][0] == 'i') type_a = 'i';
                else type_a = 'd';
            }

            if (((mas_inp[l][0] == '-') || (mas_inp[l][0] >= '0' && mas_inp[l][0] <= '9')))
            {
                type_b = type_of_operand(mas_inp[l]);
                t_b = 1;
            }
            else
            {
                k_l = find_id(mas_id, mas_id_len, mas_inp[k]);
                if (mas_id[k_l+1][0] == 'i') type_b = 'i';
                else type_b = 'd';
            }

            if (c == '+')
            {
                if ((type_a == 'i') && (type_b == 'i'))
                {
                    int result = 0;
                    if (t_a) result += atoi(mas_inp[k]);
                    else result += (int)mas_result[k_p];
                    if (t_b) result += atoi(mas_inp[l]);
                    else result += (int)mas_result[k_l];
                    sprintf(mas_inp[i], "%d", result);
                }
                else
                {
                    double result = 0;
                    if (t_a) result += atof(mas_inp[k]);
                    else result += mas_result[k_p];
                    if (t_b) result += atof(mas_inp[l]);
                    else result += mas_result[k_l];
                    sprintf(mas_inp[i], "%f", result);
                }
            }
            else
            {
                if ((type_a == 'i') && (type_b == 'i'))
                {
                    int result = 0;
                    if (t_a) result += atoi(mas_inp[k]);
                    else result += (int)mas_result[k_p];
                    if (t_b) result -= atoi(mas_inp[l]);
                    else result -= (int)mas_result[k_l];
                    sprintf(mas_inp[i], "%d", result);
                }
                else
                {
                    double result = 0.0;
                    if (t_a) result += atof(mas_inp[k]);
                    else result += mas_result[k_p];
                    if (t_b) result -= atof(mas_inp[l]);
                    else result -= mas_result[k_l];
                    sprintf(mas_inp[i], "%f", result);
                }
            }
            mas_inp[k] = mas_inp[l] = " ";
        }
    }
    i = start;
    for (j = end - 1; j > i; --j)
    {
        if (mas_inp[j][0] == '=')
        {
            k = j - 1;
            l = j + 1;
            while (mas_inp[k][0] == ' ') --k;
            while (mas_inp[l][0] == ' ') ++l;
            double res;
            char type;
            if (((mas_inp[l][0] == '-') || (mas_inp[l][0] >= '0' && mas_inp[l][0] <= '9')))
            {
                res = atof(mas_inp[l]);
                type = type_of_operand(mas_inp[l]);
            }
            else
            {
                n = find_id(mas_id, mas_id_len, mas_inp[l]);
                res = mas_result[n];
                if (mas_id[n+1][0] == 'i')
                {
                    type = 'i';
                    sprintf(mas_inp[l], "%d", (int) res);
                }
                else
                {
                    sprintf(mas_inp[l], "%f", res);
                    type = 'd';
                }
            }
            n = find_id(mas_id, mas_id_len, mas_inp[k]);
            if (n == -1) mas_id_len = add_to_mas(mas_id, mas_id_len, mas_result, mas_inp[k], res, type);
            else
            {
                if (mas_id[n+1][0] == 'i') mas_result[n] = (int) res;
                else mas_result[n] = res;
            }
            mas_inp[k] = " ";
            mas_inp[j] = " ";
        }
    }
    return mas_id_len;
}



void print_result(char mas_id[SMAX][SMAX], int mas_len, double* mas_result)
{
    for (int i=0; i < mas_len; i += 2)
    {
        if (mas_id[i+1][0] == 'd') printf("%s = %.1f\t", mas_id[i], mas_result[i]);
        else printf("%s = %d\t", mas_id[i], (int)mas_result[i]);
    }
    putchar('\n');

}

int main()
{
    int mas_len = 0, k = 0, n;
    double result = 0;
    char type;
    char mas_id[SMAX][SMAX];
    double mas_result[] = {0};
    char first_word[SMAX], del_word[SMAX];
    printf("\nPlease, separate numbers, variables and operations with spaces\n>");
    scanf("%s", first_word);
    while ((strncmp(first_word, "exit", 5) != 0))
    {
        if ((strncmp(first_word, "del", 4) == 0))
        {
            scanf("%s", del_word);
            n = find_id(mas_id, mas_len, del_word);
            k = 0;
            while (mas_id[n][k]!='\0')
            {
                mas_id[n][k] = mas_id[mas_len-2][k];
                mas_id[mas_len-2][k]='\0';
                k++;
            }
            mas_id[n+1][0] = mas_id[mas_len-1][0];
            mas_result[n] = mas_result[mas_len-2];
            mas_len-=2;
        }
        else
        {
            //scanf(" %s", del_word);
            k = 0;
            char inp_str[SMAX];
            char* mas_input[SMAX]={};
            fgets(inp_str, sizeof(inp_str), stdin);
            strcat(first_word, inp_str);
            strncpy(inp_str, first_word, SMAX);
            char* tok = strtok(inp_str, " ");
            while (tok != NULL)
            {
                mas_input[k] = tok;
                ++k;
                tok = strtok(NULL, " ");
            }
            mas_input[k-1][strlen(mas_input[k-1])-1]= '\0';
            {
                mas_len = calc(mas_id, mas_result, mas_input, 0, k, mas_len);
                k = 0;
                while (mas_input[k][0] == ' ') k++;
                type = type_of_operand(mas_input[k]);
                result = atof(mas_input[k]);
            }
            printf("Result: ");
            if (type == 'd') printf("%.1f\n", result);
            else printf("%d\n", (int) result);
        }
        print_result(mas_id, mas_len, mas_result);
        printf(">");
        scanf("%s", first_word);
    }
    return 0;
}
