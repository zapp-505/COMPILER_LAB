
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

char input[100];
int pos = 0;

void parse_E();
void parse_E_prime();
void parse_T();
void parse_T_prime();
void parse_F();

void error() {
    printf("\n[ERROR] Syntax error at position %d (symbol: '%c')\n",
           pos, input[pos]);
    exit(1);
}

void match(char expected) {
    if (input[pos] == expected) {
        pos++;
    } else {
        error();
    }
}

void parse_F() {
    if (isalnum((unsigned char)input[pos])) {
        pos++;
    } else if (input[pos] == '(') {
        match('(');
        parse_E();
        match(')');
    } else {
        error();
    }
}

void parse_T_prime() {
    if (input[pos] == '*') {
        match('*');
        parse_F();
        parse_T_prime();
    }
}

void parse_T() {
    parse_F();
    parse_T_prime();
}

void parse_E_prime() {
    if (input[pos] == '+') {
        match('+');
        parse_T();
        parse_E_prime();
    }
}

void parse_E() {
    parse_T();
    parse_E_prime();
}

int main() {
    printf("Enter expression: ");

    if (scanf("%99s", input) != 1) {
        printf("Invalid input\n");
        return 1;
    }

    parse_E();

    if (input[pos] == '\0') {
        printf("\n[SUCCESS] Expression successfully parsed and accepted!\n");
    } else {
        printf("\n[ERROR] Extra unparsed characters left at end\n");
    }

    return 0;
}
