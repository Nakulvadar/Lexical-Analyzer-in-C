#include "lexical.h"

#define STACK_SIZE 100
char sym_stack[STACK_SIZE];
int top = -1;

int read_validation(char *argv[], lexical *lexi)
{
    if (argv[1] == NULL)
    {
        printf("Error : No input file provided\n");
        return FAILURE;
    }

    lexi->ptr = fopen(argv[1], "r");
    if (lexi->ptr == NULL)
    {
        printf("Error : file not open.\n");
        return FAILURE;
    }

    strcpy(lexi->filename, argv[1]);

    char *ext = strrchr(lexi->filename, '.');
    if (ext == NULL || strcmp(ext, ".c") != 0)
    {
        printf("Error : Only .c files are allowed\n");
        fclose(lexi->ptr);
        return FAILURE;
    }

    fclose(lexi->ptr);
    return SUCCESS;
}

int start_lexical_analysis(lexical *lexi)
{
    int i = 0;
    int ch;

    lexi->ptr = fopen(lexi->filename, "r");
    rewind(lexi->ptr);

    printf("--------------------------------------------------\n");
    printf("| %-18s | %-25s |\n", "TOKEN TYPE", "LEXEME");
    printf("--------------------------------------------------\n");

    while ((ch = fgetc(lexi->ptr)) != EOF)
    {
        i = 0;
        /* Skip preprocessor lines */
        if (ch == '#')
        {
            while (ch != '\n' && ch != EOF)
                ch = fgetc(lexi->ptr);
            continue;
        }
        /* Identifier / Keyword */
        if (isalpha(ch))
        {
            lexi->buffer[i++] = ch;
            while (isalnum(ch = fgetc(lexi->ptr)))
                lexi->buffer[i++] = ch;

            lexi->buffer[i] = '\0';
            i = 0;
            fseek(lexi->ptr, -1, SEEK_CUR);

            if (check_keyword(lexi->buffer) == SUCCESS)
                printf("| %-18s | %-25s |\n", "Keyword", lexi->buffer);
            else
                printf("| %-18s | %-25s |\n", "Identifier", lexi->buffer);
        }
        else if (isspace(ch))
        {
            continue;
        }

        /* Symbols */
        else if (ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}')
        {
            if (ch == '(' || ch == '{' || ch == '[')
            {
                push(ch);
                printf("| %-18s | %-25c |\n", "Symbol", ch);
            }
            else if (ch == ')' || ch == '}' || ch == ']')
            {
                char open = pop();

                if (!is_matching(open, ch))
                {
                    printf("| %-18s | %-25c |\n",
                        "Error:Symbol mismatch", ch);
                    return FAILURE;
                }

                printf("| %-18s | %-25c |\n", "Symbol", ch);
            }
            else
            {
                printf("| %-18s | %-25c |\n", "Symbol", ch);
            }
        }

        /* Numbers */
        else if (isdigit(ch))
        {
            if (ch == '0')
            {
                int invalid =0,xvalid =0;
                lexi->buffer[i++] = ch;
                ch = fgetc(lexi->ptr);
                if (ch == '.')
                {
                    int has_dot = 1;
                    lexi->buffer[i++] = ch;

                    while ((ch = fgetc(lexi->ptr)) != EOF)
                    {
                        if (isdigit(ch))
                        {
                            lexi->buffer[i++] = ch;
                        }
                        else if (isalnum(ch))
                        {
                            lexi->buffer[i++] = ch;
                            lexi->buffer[i] = '\0';
                            printf("| %-18s | %-25s |\n",
                                "Error:Invalid numeric", lexi->buffer);
                            return FAILURE;
                        }
                        else
                        {
                            break;
                        }
                    }

                    lexi->buffer[i] = '\0';
                    printf("| %-18s | %-25s |\n",
                        "Numeric(float)", lexi->buffer);

                    if (ch != EOF)
                        fseek(lexi->ptr, -1, SEEK_CUR);
                }
                else if (ch == 'x' || ch == 'X')
                {
                    lexi->buffer[i++] = ch;
                    while ((ch = fgetc(lexi->ptr))!= EOF){
                        if(isxdigit(ch)){
                            lexi->buffer[i++] = ch;
                            xvalid =1;
                        }
                        else if(isalnum(ch)){
                            lexi->buffer[i++] = ch;
                            invalid =1;
                        }
                        else{
                            break;
                        }
                    }
                        
                    lexi->buffer[i] = '\0';
                    if(xvalid == 0 || invalid ){
                        printf("| %-18s | %-25s |\n", "Error:Numeric(hex)", lexi->buffer);
                        return FAILURE;
                    }
                    printf("| %-18s | %-25s |\n", "Numeric(hex)", lexi->buffer);
                    fseek(lexi->ptr, -1, SEEK_CUR);
                }
                else if (ch == 'b' || ch == 'B')
                {
                    lexi->buffer[i++] = ch;
                    int Binary =0,invallid_B=0;
                    while ((ch = fgetc(lexi->ptr))!= EOF){
                        if(ch == '0' || ch == '1'){
                            lexi->buffer[i++] = ch;
                            Binary =1;
                        }
                        else if(isalnum(ch)){
                            lexi->buffer[i++] = ch;
                            invallid_B =1;
                        }
                        else{
                            break;
                        }
                    }
                    lexi->buffer[i] = '\0';
                    if(Binary == 0 || invallid_B){
                        printf("| %-18s | %-25s |\n", "Error: Numeric(binary)", lexi->buffer);
                        return FAILURE;
                    }
                    printf("| %-18s | %-25s |\n", "Numeric(binary)", lexi->buffer);
                    fseek(lexi->ptr, -1, SEEK_CUR);
                }
                else if (ch >= '0' && ch <= '7')
                {
                    lexi->buffer[i++] = ch;
                    int invalid_o =0;
                    while ((ch = fgetc(lexi->ptr))  != EOF){
                        if(ch >= '0' && ch <= '7'){
                            lexi->buffer[i++] = ch;
                        }
                        else if(isalnum(ch)){
                            lexi->buffer[i++] = ch;
                            invalid_o =1;
                        }
                        else{
                            break;
                        }
                    }
                        
                    lexi->buffer[i] = '\0';
                    if(invalid_o ){
                        printf("| %-18s | %-25s |\n", "Error :Numeric(octal)", lexi->buffer);
                        return FAILURE;
                    }
                    printf("| %-18s | %-25s |\n", "Numeric(octal)", lexi->buffer);
                    fseek(lexi->ptr, -1, SEEK_CUR);
                }
                else{
                    fseek(lexi->ptr, -1, SEEK_CUR);
                    lexi->buffer[i] = '\0';
                    printf("| %-18s | %-25s |\n","Numeric(decimal)", lexi->buffer);
                }
            }
            else
            {
                int has_dot = 0;
                lexi->buffer[i++] = ch;

                while ((ch = fgetc(lexi->ptr)) != EOF)
                {
                    if (isdigit(ch))
                    {
                        lexi->buffer[i++] = ch;
                    }
                    else if (ch == '.' && !has_dot)
                    {
                        has_dot = 1;
                        lexi->buffer[i++] = ch;
                    }
                    else if (isalnum(ch))
                    {
                        lexi->buffer[i++] = ch;
                        lexi->buffer[i] = '\0';
                        printf("| %-18s | %-25s |\n",
                            "Error:Invalid numeric", lexi->buffer);
                        return FAILURE;
                    }
                    else
                    {
                        break;
                    }
                }

                lexi->buffer[i] = '\0';

                if (has_dot)
                    printf("| %-18s | %-25s |\n", "Numeric(float)", lexi->buffer);
                else
                    printf("| %-18s | %-25s |\n", "Numeric(decimal)", lexi->buffer);

                if (ch != EOF)
                    fseek(lexi->ptr, -1, SEEK_CUR);
            }
            i = 0;
        }
        /* Floating number starting with '.' */
        else if (ch == '.')
        {
            int digit_after_dot = 0;
            i = 0;
            lexi->buffer[i++] = ch;

            ch = fgetc(lexi->ptr);

            if (!isdigit(ch))
            {
                printf("| %-18s | %-25c |\n", "Error:Invalid token", '.');
                return FAILURE;
            }

            while (isdigit(ch))
            {
                digit_after_dot = 1;
                lexi->buffer[i++] = ch;
                ch = fgetc(lexi->ptr);
            }

            lexi->buffer[i] = '\0';

            if (!digit_after_dot)
            {
                printf("| %-18s | %-25s |\n",
                    "Error:Invalid float", lexi->buffer);
                return FAILURE;
            }

            printf("| %-18s | %-25s |\n",
                "Numeric(float)", lexi->buffer);

            if (ch != EOF)
                fseek(lexi->ptr, -1, SEEK_CUR);
        }
        /* Comments Handling */
        else if (ch == '/')
        {
            int next = fgetc(lexi->ptr);

            /* Single line comment */
            if (next == '/')
            {
                while ((ch = fgetc(lexi->ptr)) != '\n' && ch != EOF);
                continue;
            }

            /* Multi-line comment */
            else if (next == '*')
            {
                int prev = 0;
                int terminated = 0;

                while ((ch = fgetc(lexi->ptr)) != EOF)
                {
                    if (prev == '*' && ch == '/')
                    {
                        terminated = 1;
                        break;
                    }
                    prev = ch;
                }

                if (!terminated)
                {
                    printf("Error: Unterminated multi-line comment\n");
                    return FAILURE;
                }
                continue;
            }

            /* Not a comment, treat as division operator */
            else
            {
                ungetc(next, lexi->ptr);
                printf("| %-18s | %-25c |\n", "Operator", '/');
            }
        }

            
        /* Operators */
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '<' ||
                 ch == '>' || ch == '|' || ch == '&' || ch == '^' ||
                 ch == '!' || ch == '~' || ch == '%' || ch == '=')
        {
            char op[3] = {0};
            int next_ch = fgetc(lexi->ptr);

            op[0] = ch;
            if (next_ch == ch || next_ch == '=')
                op[1] = next_ch;
            else
                ungetc(next_ch, lexi->ptr);

            printf("| %-18s | %-25s |\n", "Operator", op);
        }
        else if (ch == '"')
        {
            int terminated = 0;
            i = 0;

            lexi->buffer[i++] = ch;

            while ((ch = fgetc(lexi->ptr)) != EOF)
            {
                lexi->buffer[i++] = ch;

                if (ch == '\\')
                {
                    ch = fgetc(lexi->ptr);
                    if (ch == EOF)
                        break;

                    lexi->buffer[i++] = ch;
                    continue;
                }

                if (ch == '"')
                {
                    terminated = 1;
                    break;
                }
                
            }

            lexi->buffer[i] = '\0';

            if (!terminated)
            {
                printf("Error: unterminated string literal\n");
                return FAILURE;
            }

            printf("| %-18s | %-25s |\n",
                "String Literal", lexi->buffer);

            i = 0;
        }
        
    }
    printf("--------------------------------------------------\n");

    if (!check_unclosed_symbols())
    {
        fclose(lexi->ptr);
        return FAILURE;
    }

    fclose(lexi->ptr);
    return SUCCESS;
}

int check_keyword(const char *keyword)
{
    const char *keywords[] =
    {
        "auto","register","static","extern","const","volatile","char","short",
        "int","long","float","double","signed","unsigned","void","if","else",
        "for","while","do","switch","case","default","break","continue",
        "return","sizeof","typedef","struct","union","enum"
    };

    int count = sizeof(keywords) / sizeof(keywords[0]);

    for (int i = 0; i < count; i++)
    {
        if (strcmp(keyword, keywords[i]) == 0)
            return SUCCESS;
    }

    return FAILURE;
}

void push(char ch)
{
    if (top < STACK_SIZE - 1)
        sym_stack[++top] = ch;
}

char pop()
{
    if (top >= 0)
        return sym_stack[top--];

    return '\0';
}

int is_matching(char open, char close)
{
    if (open == '(' && close == ')')
        return 1;
    if (open == '{' && close == '}')
        return 1;
    if (open == '[' && close == ']')
        return 1;

    return 0;
}
int check_unclosed_symbols()
{
    if (top != -1)
    {
        printf("Error: Unclosed symbol '%c'\n", sym_stack[top]);
        return 0;
    }
    return 1;
}
