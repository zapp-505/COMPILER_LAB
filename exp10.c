
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 10

struct Expr {
    char result[20];
    char op1[20];
    char op2[20];
    char op;
    int binary;
};

struct Expr arr[MAX];
int n;

int known[256];
int value[256];

int isNumber(const char *s) {
    if (*s == '\0') return 0;

    for (int i = 0; s[i]; i++) {
        if (!isdigit((unsigned char)s[i])) return 0;
    }

    return 1;
}

void replaceConstant(char *operand) {
    if (strlen(operand) == 1 &&
        isalpha((unsigned char)operand[0]) &&
        known[(unsigned char)operand[0]]) {
        sprintf(operand, "%d", value[(unsigned char)operand[0]]);
    }
}

int main(void) {
    char line[100];

    printf("Enter the maximum number of expressions: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX) {
        printf("Invalid number of expressions (1 to 10).\n");
        return 1;
    }

    getchar();

    printf("Enter the input:\n");

    for (int i = 0; i < n; i++) {
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("Invalid input.\n");
            return 1;
        }

        line[strcspn(line, "\r\n")] = '\0';

        char *equal = strchr(line, '=');
        if (equal == NULL) {
            printf("Invalid expression: %s\n", line);
            return 1;
        }

        *equal = '\0';
        strcpy(arr[i].result, line);

        char rhs[50];
        strcpy(rhs, equal + 1);

        arr[i].binary = 0;
        arr[i].op = '\0';
        strcpy(arr[i].op1, rhs);
        strcpy(arr[i].op2, "");

        for (int j = 1; rhs[j] != '\0'; j++) {
            if (rhs[j] == '+' || rhs[j] == '-' ||
                rhs[j] == '*' || rhs[j] == '/') {
                arr[i].op = rhs[j];
                rhs[j] = '\0';
                strcpy(arr[i].op1, rhs);
                strcpy(arr[i].op2, rhs + j + 1);
                arr[i].binary = 1;
                break;
            }
        }

        replaceConstant(arr[i].op1);

        if (arr[i].binary) {
            replaceConstant(arr[i].op2);

            if (isNumber(arr[i].op1) && isNumber(arr[i].op2)) {
                int a = atoi(arr[i].op1);
                int b = atoi(arr[i].op2);
                int result = 0;

                switch (arr[i].op) {
                    case '+': result = a + b; break;
                    case '-': result = a - b; break;
                    case '*': result = a * b; break;
                    case '/':
                        if (b == 0) {
                            printf("Error: division by zero.\n");
                            return 1;
                        }
                        result = a / b;
                        break;
                }

                sprintf(arr[i].op1, "%d", result);
                arr[i].binary = 0;
            }
        }

        if (!arr[i].binary && isNumber(arr[i].op1)) {
            known[(unsigned char)arr[i].result[0]] = 1;
            value[(unsigned char)arr[i].result[0]] = atoi(arr[i].op1);
        } else {
            known[(unsigned char)arr[i].result[0]] = 0;
        }
    }

    printf("\nOptimized code is:\n");

    for (int i = 0; i < n; i++) {
        if (arr[i].binary) {
            printf("%s = %s %c %s\n",
                   arr[i].result, arr[i].op1,
                   arr[i].op, arr[i].op2);
        } else {
            printf("%s = %s\n", arr[i].result, arr[i].op1);
        }
    }

    return 0;
}
