
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX 20
#define LEN 10

int n;
char productions[MAX][LEN];
char first[26][MAX];
char follow[26][MAX];

void addToSet(char set[], char val) {
    int i;

    for (i = 0; set[i] != '\0'; i++) {
        if (set[i] == val)
            return;
    }

    set[i] = val;
    set[i + 1] = '\0';
}

int contains(char set[], char val) {
    return strchr(set, val) != NULL;
}

/* Calculate FIRST sets until no set changes */
void findFirstSets(void) {
    int changed;

    do {
        changed = 0;

        for (int i = 0; i < n; i++) {
            char lhs = productions[i][0];
            char *rhs = productions[i] + 2;
            int lhsIdx = lhs - 'A';

            if (rhs[0] == '#') {
                if (!contains(first[lhsIdx], '#')) {
                    addToSet(first[lhsIdx], '#');
                    changed = 1;
                }
                continue;
            }

            int allNullable = 1;

            for (int j = 0; rhs[j] != '\0'; j++) {
                char symbol = rhs[j];

                if (!isupper((unsigned char)symbol)) {
                    if (!contains(first[lhsIdx], symbol)) {
                        addToSet(first[lhsIdx], symbol);
                        changed = 1;
                    }

                    allNullable = 0;
                    break;
                }

                int idx = symbol - 'A';

                for (int k = 0; first[idx][k] != '\0'; k++) {
                    char val = first[idx][k];

                    if (val != '#' &&
                        !contains(first[lhsIdx], val)) {
                        addToSet(first[lhsIdx], val);
                        changed = 1;
                    }
                }

                if (!contains(first[idx], '#')) {
                    allNullable = 0;
                    break;
                }
            }

            if (allNullable &&
                !contains(first[lhsIdx], '#')) {
                addToSet(first[lhsIdx], '#');
                changed = 1;
            }
        }
    } while (changed);
}

/* Calculate FOLLOW sets until no set changes */
void findFollowSets(void) {
    int changed;

    addToSet(follow[productions[0][0] - 'A'], '$');

    do {
        changed = 0;

        for (int i = 0; i < n; i++) {
            char lhs = productions[i][0];
            char *rhs = productions[i] + 2;
            int lhsIdx = lhs - 'A';

            for (int j = 0; rhs[j] != '\0'; j++) {
                char symbol = rhs[j];

                if (!isupper((unsigned char)symbol))
                    continue;

                int idx = symbol - 'A';
                int nullable = 1;

                for (int k = j + 1; rhs[k] != '\0'; k++) {
                    char next = rhs[k];

                    if (!isupper((unsigned char)next)) {
                        if (!contains(follow[idx], next)) {
                            addToSet(follow[idx], next);
                            changed = 1;
                        }

                        nullable = 0;
                        break;
                    }

                    int nextIdx = next - 'A';

                    for (int m = 0;
                         first[nextIdx][m] != '\0'; m++) {
                        char val = first[nextIdx][m];

                        if (val != '#' &&
                            !contains(follow[idx], val)) {
                            addToSet(follow[idx], val);
                            changed = 1;
                        }
                    }

                    if (!contains(first[nextIdx], '#')) {
                        nullable = 0;
                        break;
                    }
                }

                if (nullable) {
                    for (int k = 0;
                         follow[lhsIdx][k] != '\0'; k++) {
                        char val = follow[lhsIdx][k];

                        if (!contains(follow[idx], val)) {
                            addToSet(follow[idx], val);
                            changed = 1;
                        }
                    }
                }
            }
        }
    } while (changed);
}

int main(void) {
    printf("Enter number of productions: ");

    if (scanf("%d", &n) != 1 || n < 1 || n >= MAX) {
        printf("Invalid number of productions.\n");
        return 1;
    }

    printf("Enter rules (example: E=TR, # means epsilon):\n");

    for (int i = 0; i < n; i++) {
        if (scanf("%9s", productions[i]) != 1 ||
            strlen(productions[i]) < 3 ||
            productions[i][1] != '=' ||
            !isupper((unsigned char)productions[i][0])) {
            printf("Invalid production format.\n");
            return 1;
        }
    }

    findFirstSets();
    findFollowSets();

    printf("\n--- FIRST SETS ---\n");

    for (int i = 0; i < n; i++) {
        int idx = productions[i][0] - 'A';

        printf("FIRST(%c) = { ", productions[i][0]);

        for (int j = 0; first[idx][j] != '\0'; j++)
            printf("%c ", first[idx][j]);

        printf("}\n");
    }

    printf("\n--- FOLLOW SETS ---\n");

    for (int i = 0; i < n; i++) {
        int idx = productions[i][0] - 'A';

        printf("FOLLOW(%c) = { ", productions[i][0]);

        for (int j = 0; follow[idx][j] != '\0'; j++)
            printf("%c ", follow[idx][j]);

        printf("}\n");
    }

    return 0;
}
