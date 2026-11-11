
#include <stdio.h>

#define SIZE 50

char res[SIZE][10];
char opnd1[SIZE][10];
char opnd2[SIZE][10];
char op[SIZE];

int num;

int main() {
    int i;
    char eq[10], opstr[10];

    printf("Enter the number of instructions: ");
    scanf("%d", &num);

    if (num > SIZE) {
        printf("Too many instructions (max %d)\n", SIZE);
        return 1;
    }

    printf("Enter each instruction (Example: t1 = a + b)\n");

    for (i = 0; i < num; i++) {
        scanf("%9s %9s %9s %9s %9s",
              res[i], eq, opnd1[i], opstr, opnd2[i]);

        op[i] = opstr[0];
    }

    printf("\nEquivalent 8086 Assembly Code:\n");

    for (i = 0; i < num; i++) {
        switch (op[i]) {

            case '+':
                printf("\nMOV AX, %s", opnd1[i]);
                printf("\nADD AX, %s", opnd2[i]);
                printf("\nMOV %s, AX\n", res[i]);
                break;

            case '-':
                printf("\nMOV AX, %s", opnd1[i]);
                printf("\nSUB AX, %s", opnd2[i]);
                printf("\nMOV %s, AX\n", res[i]);
                break;

            case '*':
                printf("\nMOV AX, %s", opnd1[i]);
                printf("\nMOV BX, %s", opnd2[i]);
                printf("\nMUL BX");
                printf("\nMOV %s, AX\n", res[i]);
                break;

            case '/':
                printf("\nMOV AX, %s", opnd1[i]);
                printf("\nMOV BX, %s", opnd2[i]);
                printf("\nXOR DX, DX");
                printf("\nDIV BX");
                printf("\nMOV %s, AX\n", res[i]);
                break;

            default:
                printf("\nInvalid operator: %c\n", op[i]);
        }
    }

    return 0;
}
