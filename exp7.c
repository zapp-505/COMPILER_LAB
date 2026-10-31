
#include <stdio.h>

char prod[20][20];       // Stores all productions
int n;                   // Number of productions

char set[30];            // Stores FIRST or FOLLOW set
int m;                   // Number of symbols in the set

int busy[128];           // Prevents infinite recursion in FIRST
int seen[128];           // Prevents infinite recursion in FOLLOW

// Add a symbol only if it is not already present
void add(char c) {
    for (int i = 0; i < m; i++) {
        if (set[i] == c)
            return;
    }
    set[m++] = c;
}

// Find FIRST(c)
// Returns 1 if c can derive epsilon, otherwise 0
int first(char c) {

    // If c is a terminal, add it
    if (c < 'A' || c > 'Z') {
        add(c);
        return 0;
    }

    // Stop if this non-terminal is already being processed
    if (busy[c])
        return 0;

    busy[c] = 1;
    int eps = 0;

    // Check every production of c
    for (int i = 0; i < n; i++) {
        if (prod[i][0] != c)
            continue;

        int allvanish = 1;

        // Examine RHS from left to right
        for (int j = 2; prod[i][j] != '\0'; j++) {

            // # represents epsilon
            if (prod[i][j] == '#') {
                eps = 1;
                break;
            }

            // Continue only if this symbol can derive epsilon
            if (!first(prod[i][j])) {
                allvanish = 0;
                break;
            }
        }

        // Every symbol on RHS can derive epsilon
        if (allvanish)
            eps = 1;
    }

    busy[c] = 0;
    return eps;
}

// Find FOLLOW(c)
void follow(char c) {

    // Avoid processing the same non-terminal repeatedly
    if (seen[c])
        return;

    seen[c] = 1;

    // The start symbol always contains $ in its FOLLOW set
    if (prod[0][0] == c)
        add('$');

    // Search for c in  every RHS
    for (int i = 0; i < n; i++) {
        for (int j = 2; prod[i][j] != '\0'; j++) {

            if (prod[i][j] != c)
                continue;

            int allvanish = 1;

            // Check the symbols appearing after c
            for (int k = j + 1; prod[i][k] != '\0'; k++) {

                // Epsilon does not contribute a terminal
                if (prod[i][k] == '#')
                    continue;

                // FIRST of the next symbol is added to set
                if (!first(prod[i][k])) {
                    allvanish = 0;
                    break;
                }
            }

            // If c is at the end, or everything after it
            // can derive epsilon, add FOLLOW of the LHS
            if (allvanish)
                follow(prod[i][0]);
        }
    }
}

// Display the calculated set
void show(char name[], char c) {
    printf("%s(%c) = { ", name, c);

    for (int i = 0; i < m; i++)
        printf("%c ", set[i]);

    printf("}\n");
}

int main() {
    char c;
    int again = 1;

    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (use # for epsilon):\n");

    // Enter each alternative on a separate line
    for (int i = 0; i < n; i++)
        scanf("%s", prod[i]);

    while (again == 1) {

        printf("\nEnter the non-terminal: ");
        scanf(" %c", &c);

        // -------- FIRST --------
        m = 0;

        // Reset recursion flags
        for (int i = 0; i < 128; i++)
            busy[i] = 0;

        // Add epsilon to FIRST if it can be derived
        if (first(c))
            add('#');

        show("First", c);

        // -------- FOLLOW --------
        m = 0;

        // Reset flags before calculating FOLLOW
        for (int i = 0; i < 128; i++) {
            seen[i] = 0;
            busy[i] = 0;
        }

        follow(c);
        show("Follow", c);

        printf("\nEnter 1 to continue: ");
        scanf("%d", &again);
    }

    return 0;
}
