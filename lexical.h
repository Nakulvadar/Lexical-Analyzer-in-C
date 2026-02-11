#ifndef LEXICAL_H
#define LEXICAL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SUCCESS 1
#define FAILURE 0

typedef struct lexical
{
    char filename[100];
    FILE *ptr;
    char buffer[100];
} lexical;

int read_validation(char *argv[], lexical *lexi);
int start_lexical_analysis(lexical *lexi);
int check_keyword(const char *keyword);

void push(char ch);
char pop(void);
int is_matching(char open, char close);
int check_unclosed_symbols(void);

#endif
