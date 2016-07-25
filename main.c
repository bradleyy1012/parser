/*
 * Created by Bradley Kovacs
 * COP 3402 - Summer 2016
 * Assignment 3 - Parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX_CODE_LENGTH 500
#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_IDENTIFIERS_LENGTH 11
#define MIN_INC 4

#include "data-structures.h"

// Global variables
int tableIndex, numTokensRead, codeIndex, lexLevel, address, varOffset;
symbol *symbolTable[MAX_SYMBOL_TABLE_SIZE];
instruction code[MAX_CODE_LENGTH];
struct Token *tokenNodeHead, *currentToken;

#include "symbol-table.h"
#include "error-messages.h"
#include "parser.h"
#include "parser-states.h"

int main(int argc, const char*argv[])
{
    // Initially set some stuff
    address = 1, numTokensRead = tableIndex = codeIndex = 0, lexLevel = -1, varOffset = 4;
    tokenNodeHead = NULL;

    printf("Compilation process started\n");

    // Load in the tokens
    loadTokens();

    // Update currentToken to the first token
    getNextToken();
    block();
    emit(SIO3, 0, 3);

    if (currentToken->intData != periodsym) {
        printErrorMessage(expectedPeriod);
        haltThatShit();
    }

    printf("Compiling has been complete and no syntax errors were detected\n");

    printSymbolTable();

    int i;
    printf("\n\n\nOp:\tL\tM\n");
    for (i = 0; i < MAX_CODE_LENGTH; i++) {
        struct instruction *ele = &code[i];
        if (ele->op == 0) break;
        printf("%d\t%d\t%d\n", ele->op, ele->l, ele->m);
    }

    return 0;
}