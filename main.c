#include <stdio.h>
#include <ctype.h>
#include "parser.h"

int main(int argc, const char*argv[])
{
    lexemeListFile = fopen("lexemelist.txt", "r");

    // Initially set the table index
    tableIndex = 0;

    // Get the token
    token = getToken();
    block();

    if (token != periodsym) {
        // TODO: output error
    }

    fclose(lexemeListFile);
    printSymbolTable();

    return 0;
}