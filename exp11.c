
#include <stdio.h>
#include <string.h>

int main(void) {
    char icode[10][30];
    char str[30];
    char opr[10];
    int i = 0;

    printf("Enter the set of intermediate code "
           "(terminated by exit):\n");

    while (i < 10) {
        if (fgets(icode[i], sizeof(icode[i]), stdin) == NULL) {
            break;
        }

        icode[i][strcspn(icode[i], "\r\n")] = '\0';

        if (strcmp(icode[i], "exit") == 0) {
            break;
        }

        if (strlen(icode[i]) == 0) {
            continue;
        }

        i++;
    }

    printf("\nTarget code generation\n");
    printf("************************\n");

    for (int j = 0; j < i; j++) {
        strcpy(str, icode[j]);

        /*
           Expected input format: a=b+c
           str[0] = result
           str[2] = first operand
           str[3] = operator
           str[4] = second operand
        */

        if (strlen(str) != 5 || str[1] != '=') {
            printf("Unsupported expression: %s\n", str);
            continue;
        }

        switch (str[3]) {
            case '+':
                strcpy(opr, "ADD");
                break;

            case '-':
                strcpy(opr, "SUB");
                break;

            case '*':
                strcpy(opr, "MUL");
                break;

            case '/':
                strcpy(opr, "DIV");
                break;

            default:
                printf("Unsupported operator in: %s\n", str);
                continue;
        }

        printf("\n\tMOV %c,R%d", str[2], j);
        printf("\n\t%s %c,R%d", opr, str[4], j);
        printf("\n\tMOV R%d,%c\n", j, str[0]);
    }

    return 0;
}
