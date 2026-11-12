
#include <stdio.h>

#define MAX 50

int from[MAX], to[MAX];
char sym[MAX];
int n;
int visited[MAX];

// DFS to find epsilon closure
void dfs(int s) {
    visited[s] = 1;

    for (int i = 0; i < n; i++) {
        if (from[i] == s && sym[i] == 'e' && !visited[to[i]]) {
            dfs(to[i]);
        }
    }
}

int main() {
    int states;

    printf("Enter number of states: ");
    scanf("%d", &states);

    printf("Enter number of transitions: ");
    scanf("%d", &n);

    printf("Enter transitions (state symbol next_state), use e for epsilon:\n");

    for (int i = 0; i < n; i++) {
        scanf("%d %c %d", &from[i], &sym[i], &to[i]);
    }

    printf("\n");

    // Find epsilon closure of every state
    for (int s = 0; s < states; s++) {

        // Reset visited array
        for (int j = 0; j < MAX; j++) {
            visited[j] = 0;
        }

        // Calculate epsilon closure
        dfs(s);

        // Print epsilon closure
        printf("E-closure(q%d) = { ", s);

        for (int j = 0; j < states; j++) {
            if (visited[j]) {
                printf("q%d ", j);
            }
        }

        printf("}\n");
    }

    return 0;
}
