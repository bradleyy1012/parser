#include <stdio.h>
#include <ctype.h>
#include "parser.h"

int main(int argc, const char*argv[])
{
    lexemeListFile = fopen("lexemelist.txt", "r");
    FILE* symListFile = fopen("symlist.txt", "w");

    // Get the token
    token = getToken();
    block();

    fclose(lexemeListFile);
    fclose(symListFile);

    return 0;
}