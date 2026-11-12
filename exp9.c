
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX 100

char op_stack[MAX];
int op_top = -1;

char val_stack[MAX][10];
int val_top = -1;

int temp_count = 1;

// Function to determine operator precedence
int precedence(char op) {
    if (op == '*' || op == '/')
        return 2;

    if (op == '+' || op == '-')
        return 1;

    return 0;
}

// Function to generate TAC
void generate_tac() {
    char op, op1[10], op2[10], temp[10];

    if (val_top < 1) {
        printf("Invalid expression\n");
        exit(1);
    }

    op = op_stack[op_top--];

    strcpy(op2, val_stack[val_top--]);//arr shld be assignd w strcpy
    strcpy(op1, val_stack[val_top--]);

    sprintf(temp, "t%d", temp_count++);

    printf("%s = %s %c %s\n", temp, op1, op, op2);

    strcpy(val_stack[++val_top], temp);
}

// Function to convert infix expression into TAC
void parse_to_tac(char *expr) {
    int i = 0, k;
    char val[10];

    while (expr[i] != '\0' && expr[i] != '\n') {

        if (expr[i] == ' ') {
            i++;
        }

        else if (isalnum(expr[i])) {
            k = 0;

            while (isalnum(expr[i])) {
                val[k] = expr[i];
                k++;
                i++;
            }

            val[k] = '\0';

            strcpy(val_stack[++val_top], val);
        }

        else if (expr[i] == '(') {
            op_stack[++op_top] = '(';
            i++;
        }

        else if (expr[i] == ')') {
            while (op_top >= 0 && op_stack[op_top] != '(')
                generate_tac();

            if (op_top < 0) {
                printf("Mismatched parentheses\n");
                exit(1);
            }


            op_top--;//most past opening para
            i++;//closing para
        }

        else {
            while (op_top >= 0 &&
                   precedence(op_stack[op_top]) >= precedence(expr[i]))
                generate_tac();
            //if no val then add to top 
            op_stack[++op_top] = expr[i];
            i++;
        }
    }

    while (op_top >= 0)
        generate_tac();
}

int main() {
    char expr[MAX];

    printf("Enter an expression: ");
    fgets(expr, MAX, stdin);//whitespace allowed

    printf("\nThree address code:\n");

    parse_to_tac(expr);

    return 0;
}
