
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

// Store quadruples (operator, arg1, arg2, result)
char quad[MAX][4][10];
int q_count = 0;

// Higher number = higher priority
int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

// Apply operator and store quadruple
void generate_tac() {
    char op, op1[10], op2[10], temp[10];

    if (val_top < 1) {
        printf("Invalid expression\n");
        exit(1);
    }

    op = op_stack[op_top--];

    strcpy(op2, val_stack[val_top--]);
    strcpy(op1, val_stack[val_top--]);

    // Generate temporary variable
    sprintf(temp, "t%d", temp_count++);

    // Store quadruple
    sprintf(quad[q_count][0], "%c", op);
    strcpy(quad[q_count][1], op1);
    strcpy(quad[q_count][2], op2);
    strcpy(quad[q_count][3], temp);
    q_count++;

    // Push result back onto value stack
    strcpy(val_stack[++val_top], temp);
}

// Parse arithmetic expression
void parse_to_tac(char *expr) {
    int i = 0, k;
    char val[10];

    while (expr[i] != '\0' && expr[i] != '\n') {

        if (expr[i] == ' ') {
            i++;
        }

        else if (isalnum((unsigned char)expr[i])) {
            k = 0;

            while (isalnum((unsigned char)expr[i])) {
                if (k >= 9) {
                    printf("Operand too long\n");
                    exit(1);
                }

                val[k++] = expr[i++];
            }

            val[k] = '\0';
            strcpy(val_stack[++val_top], val);
        }

        else if (expr[i] == '(') {
            op_stack[++op_top] = '(';
            i++;
        }

        else if (expr[i] == ')') {
            while (op_top >= 0 && op_stack[op_top] != '(') {
                generate_tac();
            }

            if (op_top < 0) {
                printf("Mismatched parentheses\n");
                exit(1);
            }

            op_top--;
            i++;
        }

        else if (expr[i] == '+' || expr[i] == '-' ||
                 expr[i] == '*' || expr[i] == '/') {

            while (op_top >= 0 &&
                   op_stack[op_top] != '(' &&
                   precedence(op_stack[op_top]) >= precedence(expr[i])) {
                generate_tac();
            }

            op_stack[++op_top] = expr[i++];
        }

        else {
            printf("Invalid character: %c\n", expr[i]);
            exit(1);
        }
    }

    // Apply remaining operators
    while (op_top >= 0) {
        if (op_stack[op_top] == '(') {
            printf("Mismatched parentheses\n");
            exit(1);
        }

        generate_tac();
    }
}

// Print quadruples
void print_quadruples() {
    int i;

    printf("\nQUADRUPLES\n");
    printf("Op\tArg1\tArg2\tResult\n");

    for (i = 0; i < q_count; i++) {
        printf("%s\t%s\t%s\t%s\n",
               quad[i][0], quad[i][1],
               quad[i][2], quad[i][3]);
    }
}

// Print triples
void print_triples() {
    int i, a, b;

    printf("\nTRIPLES\n");
    printf("Index\tOp\tArg1\tArg2\n");

    for (i = 0; i < q_count; i++) {
        printf("%d\t%s\t", i, quad[i][0]);

        // Print first operand
        if (quad[i][1][0] == 't')
            printf("(%d)\t", atoi(quad[i][1] + 1) - 1);
        else
            printf("%s\t", quad[i][1]);

        // Print second operand
        if (quad[i][2][0] == 't')
            printf("(%d)\n", atoi(quad[i][2] + 1) - 1);
        else
            printf("%s\n", quad[i][2]);
    }
}

int main() {
    char expr[MAX];

    printf("Enter an arithmetic expression: ");
    fgets(expr, MAX, stdin);

    parse_to_tac(expr);

    print_quadruples();
    print_triples();

    return 0;
}
