#include <stdio.h>
#include <string.h>
#include <ctype.h>

int isKeyword(char word[]) {
    char keywords[][10] = {
        "auto", "break", "case", "char", "const", "continue",
        "default", "do", "double", "else", "enum", "extern",
        "float", "for", "goto", "if", "int", "long",
        "register", "return", "short", "signed", "sizeof",
        "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while"
    };

    int count = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < count; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }

    return 0;
}

int isSeparator(int ch) {
    return strchr(";,(){}[]", ch) != NULL;
}

int isOperator(int ch) {
    return strchr("+-*/%=<>!&|^~?:.", ch) != NULL;
}

int main(void) {
    FILE *fp;
    char filename[100];
    int ch, next;

    printf("Enter the filename to analyze: ");

    if (scanf("%99s", filename) != 1) {
        printf("Invalid filename.\n");
        return 1;
    }

    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Could not open file.\n");
        return 1;
    }

    while ((ch = fgetc(fp)) != EOF) {

        /* Ignore whitespace */
        if (isspace((unsigned char)ch)) {
            continue;
        }

        /* Ignore preprocessor lines such as #include */
        if (ch == '#') {
            while ((ch = fgetc(fp)) != EOF && ch != '\n') {
                /* Skip the line */
            }
            continue;
        }

        /* Handle comments and division operator */
        if (ch == '/') {
            next = fgetc(fp);

            /* Single-line comment */
            if (next == '/') {
                while ((ch = fgetc(fp)) != EOF && ch != '\n') {
                    /* Skip comment */
                }
                continue;
            }

            /* Multi-line comment */
            if (next == '*') {
                int prev = 0;

                while ((ch = fgetc(fp)) != EOF) {
                    if (prev == '*' && ch == '/') {
                        break;
                    }
                    prev = ch;
                }
                continue;
            }

            /* Not a comment: check for /= */
            if (next == '=') {
                printf("Operator: /=\n");
            } else {
                printf("Operator: /\n");
                if (next != EOF) {
                    ungetc(next, fp);
                }
            }
            continue;
        }

        /* Handle string literals */
        if (ch == '"') {
            char word[1000];
            int i = 0;
            int escaped = 0;

            word[i++] = '"';

            while ((ch = fgetc(fp)) != EOF) {
                if (i < (int)sizeof(word) - 2) {
                    word[i++] = (char)ch;
                }

                if (ch == '"' && !escaped) {
                    break;
                }

                if (ch == '\\' && !escaped) {
                    escaped = 1;
                } else {
                    escaped = 0;
                }
            }

            word[i] = '\0';
            printf("String: %s\n", word);
            continue;
        }

        /* Handle character constants */
        if (ch == '\'') {
            char word[1000];
            int i = 0;
            int escaped = 0;

            word[i++] = '\'';

            while ((ch = fgetc(fp)) != EOF) {
                if (i < (int)sizeof(word) - 2) {
                    word[i++] = (char)ch;
                }

                if (ch == '\'' && !escaped) {
                    break;
                }

                if (ch == '\\' && !escaped) {
                    escaped = 1;
                } else {
                    escaped = 0;
                }
            }

            word[i] = '\0';
            printf("Character constant: %s\n", word);
            continue;
        }

        /* Handle identifiers and keywords */
        if (isalpha((unsigned char)ch) || ch == '_') {
            char word[1000];
            int i = 0;

            do {
                if (i < (int)sizeof(word) - 1) {
                    word[i++] = (char)ch;
                }
                ch = fgetc(fp);
            } while (ch != EOF &&
                     (isalnum((unsigned char)ch) || ch == '_'));

            word[i] = '\0';

            if (isKeyword(word)) {
                printf("Keyword: %s\n", word);
            } else {
                printf("Identifier: %s\n", word);
            }

            if (ch != EOF) {
                ungetc(ch, fp);
            }
            continue;
        }

        /* Handle numeric constants */
        if (isdigit((unsigned char)ch)) {
            char word[1000];
            int i = 0;
            int has_dot = 0;

            do {
                if (i < (int)sizeof(word) - 1) {
                    word[i++] = (char)ch;
                }

                ch = fgetc(fp);

                if (ch == '.' && !has_dot) {
                    has_dot = 1;
                } else if (!isdigit((unsigned char)ch)) {
                    break;
                }

            } while (ch != EOF &&
                     (isdigit((unsigned char)ch) ||
                      (ch == '.' && has_dot)));

            word[i] = '\0';
            printf("Constant: %s\n", word);

            if (ch != EOF) {
                ungetc(ch, fp);
            }
            continue;
        }

        /* Handle operators, including common two-character operators */
        if (isOperator(ch)) {
            char op[4] = {(char)ch, '\0', '\0', '\0'};
            next = fgetc(fp);

            if (next == '=' ||
                ((ch == '+' || ch == '-') && next == ch) ||
                ((ch == '&' || ch == '|') && next == ch) ||
                ((ch == '<' || ch == '>') && next == ch)) {

                op[1] = (char)next;

                if ((ch == '<' || ch == '>') && next == ch) {
                    int third = fgetc(fp);

                    if (third == '=') {
                        op[2] = '=';
                    } else if (third != EOF) {
                        ungetc(third, fp);
                    }
                }

            } else if (next != EOF) {
                ungetc(next, fp);
            }

            printf("Operator: %s\n", op);
            continue;
        }

        /* Handle separators */
        if (isSeparator(ch)) {
            printf("Symbol: %c\n", ch);
            continue;
        }

        /* Handle unrecognized characters */
        printf("Unknown: %c\n", ch);
    }

    fclose(fp);
    return 0;
}   