
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int getIndex(char c) {
    switch (c) {
        case '+':
        case '-':
            return 0;

        case '*':
        case '/':
            return 1;

        case 'i':
            return 2;

        case '$':
            return 3;

        default:
            return -1;
    }
}

int main() {
    char precTable[4][4] = {
        {'>', '<', '<', '>'},
        {'>', '>', '<', '>'},
        {'>', '>', ' ', '>'},
        {'<', '<', '<', '='}
    };

    char stack[100], input[100];
    int top = 0;
    int i = 0;

    stack[top] = '$';
    stack[top + 1] = '\0';

    printf("Enter input string: ");
    scanf("%99s", input);

    printf("\n%-15s %-15s %-10s %s\n",
           "Stack", "Input", "Precedence", "Action");

    printf("----------------------------------------------------------\n");

    while (1) {
        char stackTop = stack[top];
        char currentInput = input[i];

        int row = getIndex(stackTop);
        int col = getIndex(currentInput);

        if (row == -1 || col == -1) {
            printf("\nResult: Parsing Failed\n");
            break;
        }

        char relation = precTable[row][col];

        stack[top + 1] = '\0';

        printf("%-15s %-15s %-10c ",
               stack, &input[i], relation);

        if (stackTop == '$' && currentInput == '$') {
            printf("ACCEPT\n");
            printf("\nResult: Input string is Successfully Parsed!\n");
            break;
        }

        if (relation == '<' || relation == '=') {
            top++;
            stack[top] = currentInput;
            stack[top + 1] = '\0';
            i++;

            printf("Shift '%c'\n", currentInput);
        }
        else if (relation == '>') {
            char popped = stack[top];
            top--;

            printf("Reduce '%c'\n", popped);
        }
        else {
            printf("ERROR\n");
            printf("\nResult: Parsing Failed\n");
            break;
        }
    }

    return 0;
}
