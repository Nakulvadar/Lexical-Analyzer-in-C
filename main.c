#include "lexical.h"

int main(int argc , char *argv[])
{
    lexical lexi;
    
    if(argc == 2){

        if(read_validation(argv,&lexi)==SUCCESS){
            printf("Validation Successfuly.\n");

            if(start_lexical_analysis(&lexi)==SUCCESS)
            {
                printf("Lexical Analysing is complited successfuly. \n");
            }
            else{
                printf("Error : Lexical Analysing failed.\n");
                return FAILURE;
            }
        }
        else{
            printf("Error : read validation failed.\n");
            return FAILURE;
        }
    }
    else{
        printf("Error : wrong argument passsed.\n");
        printf("  ./a.out  <file_name.>\n");
        return FAILURE;
    }
    printf("Thank you.\n");
    return SUCCESS;
}