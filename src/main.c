#include <stdio.h>
#include "include/lexer.h"

int main(int argc, char* argv[]){
    lexer_T* lexer = init_lexer(
        "var name = \"John Doe\";\n"
        "print(name);\n"
    );


    token_T* token = NULL; 

    
    while ((token = lexer_get_next_token(lexer)) != NULL) {
        
        if (token && token->value) {
             printf("TOKEN(%d, %s)\n", token->type, token->value);
        } else if (token) {
             
             printf("TOKEN(%d, (null value))\n", token->type);
        }
        
    }
}