/*
change: whenever a variable and value is discovered, from that statement onwards
replace the variale with the value
When RHS is already a constant, propagate it and skip the remaining processing.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char statements[10][50];
int n;

// Replace a variable with its constant value in later statements
//iterate till the var, store till var to temp and add \0 in posi of temp
//concatinate value to temp and also remaining part of statement to temp 
//replace staetment with temp, then move to locatio after temp
void change(int pos, char variable, char *value)
{
    int i, j;

    for (i = pos + 1; i < n; i++)
    {
        for (j = 2; statements[i][j] != '\0'; j++)
        {
            if (statements[i][j] == variable)
            {
                // Replace the variable with its constant value
                char temp[50];

                strncpy(temp, statements[i], j);
                temp[j] = '\0';

                strcat(temp, value);
                strcat(temp, statements[i] + j + 1);

                strcpy(statements[i], temp);
                j += strlen(value) - 1;
            }
        }
    }
}

// Perform constant propagation and constant folding
void constant()
{
    int i, j, left, right, result;
    char variable, op;
    char rhs[50], value[20];

    for (i = 0; i < n; i++)
    {
        variable = statements[i][0];
        strcpy(rhs, statements[i] + 2);

        // Check whether the right side contains only a number
        if (isdigit(rhs[0]))
        {
            int isNumber = 1;

            for (j = 0; rhs[j] != '\0'; j++)
            {
                if (!isdigit(rhs[j]))
                    isNumber = 0;
            }

            if (isNumber)
            {
                strcpy(value, rhs);
                change(i, variable, value);
                continue;
            }
        }

        // Find the arithmetic operator
        for (j = 0; rhs[j] != '\0'; j++)
        {
            if (rhs[j] == '+' || rhs[j] == '-' ||
                rhs[j] == '*' || rhs[j] == '/')
                break;
        }

        // Fold expressions containing two constants
        //first check if operator existed i.e it didnt reach end
        //then check if both are digits then perform operation
        if (rhs[j] != '\0')
        {
            op = rhs[j];
            rhs[j] = '\0';

            if (isdigit(rhs[0]) && isdigit(rhs[j + 1]))
            {
                left = atoi(rhs);
                right = atoi(rhs + j + 1);

                if (op == '/' && right == 0)
                    continue;

                switch (op)
                {
                    case '+': result = left + right; break;
                    case '-': result = left - right; break;
                    case '*': result = left * right; break;
                    case '/': result = left / right; break;
                    default: continue;
                }

                sprintf(value, "%d", result);
                sprintf(statements[i], "%c=%s", variable, value);

                // Propagate the newly calculated constant
                change(i, variable, value);
            }
        }
    }
}

// Display optimized code
void output()
{
    int i, j;

    printf("\nOptimized code:\n");

    for (i = 0; i < n; i++)
    {
        printf("%c = ", statements[i][0]);

        for (j = 2; statements[i][j] != '\0'; j++)
            printf("%c", statements[i][j]);

        printf("\n");
    }
}

int main()
{
    int i;

    printf("Enter number of statements: ");
    scanf("%d", &n);

    printf("Enter statements without spaces:\n");

    for (i = 0; i < n; i++)
        scanf("%49s", statements[i]);

    constant();
    output();

    return 0;
}
