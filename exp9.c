
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Instruction {
    char op;
    char arg1[10];
    char arg2[10];
    char result[10];
};

int tempCount = 1;

void generateTAC(char *expr) {
    struct Instruction code[20];
    int codeIndex = 0;
    char str[100];

    strcpy(str, expr);
    int len = strlen(str);

    /* Process multiplication and division first */
    for (int i = 0; i < len; i++) {
        if (str[i] == '*' || str[i] == '/') {
            int left = i - 1;

            while (left >= 0 &&
                   (isalnum((unsigned char)str[left]) ||
                    str[left] == 't')) {
                left--;
            }
            left++;

            int right = i + 1;

            while (right < len &&
                   (isalnum((unsigned char)str[right]) ||
                    str[right] == 't')) {
                right++;
            }

            char op1[10] = {0}, op2[10] = {0};

            strncpy(op1, &str[left], i - left);
            strncpy(op2, &str[i + 1], right - (i + 1));

            char tempVar[10];
            sprintf(tempVar, "t%d", tempCount++);

            code[codeIndex].op = str[i];
            strcpy(code[codeIndex].arg1, op1);
            strcpy(code[codeIndex].arg2, op2);
            strcpy(code[codeIndex].result, tempVar);
            codeIndex++;

            char buffer[100] = {0};

            strncpy(buffer, str, left);
            strcat(buffer, tempVar);
            strcat(buffer, &str[right]);

            strcpy(str, buffer);
            len = strlen(str);
            i = -1;
        }
    }

    len = strlen(str);

    /* Process addition and subtraction */
    for (int i = 0; i < len; i++) {
        if (str[i] == '+' || str[i] == '-') {
            int left = i - 1;

            while (left >= 0 &&
                   (isalnum((unsigned char)str[left]) ||
                    str[left] == 't')) {
                left--;
            }
            left++;

            int right = i + 1;

            while (right < len &&
                   (isalnum((unsigned char)str[right]) ||
                    str[right] == 't')) {
                right++;
            }

            char op1[10] = {0}, op2[10] = {0};

            strncpy(op1, &str[left], i - left);
            strncpy(op2, &str[i + 1], right - (i + 1));

            char tempVar[10];
            sprintf(tempVar, "t%d", tempCount++);

            code[codeIndex].op = str[i];
            strcpy(code[codeIndex].arg1, op1);
            strcpy(code[codeIndex].arg2, op2);
            strcpy(code[codeIndex].result, tempVar);
            codeIndex++;

            char buffer[100] = {0};

            strncpy(buffer, str, left);
            strcat(buffer, tempVar);
            strcat(buffer, &str[right]);

            strcpy(str, buffer);
            len = strlen(str);
            i = -1;
        }
    }

    printf("\nGenerated Three Address Code (TAC):\n");
    printf("-----------------------------------\n");

    for (int k = 0; k < codeIndex; k++) {
        printf("%s = %s %c %s\n",
               code[k].result,
               code[k].arg1,
               code[k].op,
               code[k].arg2);
    }
}

int main() {
    char expr[100];

    printf("Enter expression: ");
    scanf("%99s", expr);

    generateTAC(expr);

    return 0;
}
