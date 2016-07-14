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

#include "data-structures.h"
#include "error-messages.h"

// Global variables
int tableIndex, numTokensRead, codeIndex;
symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
instruction code[MAX_CODE_LENGTH];
struct Token *tokenNodeHead, *currentToken; // currentToken is the current lexeme being processed

#include "parser-helper.h"
#include "parser.h"

int main(int argc, const char*argv[])
{
    // Initially set some stuff
    numTokensRead = 0, tableIndex = 0, codeIndex = 0;
    tokenNodeHead = NULL;

    printf("Compilation process started\n");

    // Load in the tokens
    loadTokens();

    // Update currentToken to the first token
    getNextToken();
    block();

    if (currentToken->intData != periodsym) {
        printErrorMessage(expectedPeriod);
        haltThatShit();
    }

    printf("Compiling has been complete and no syntax errors were detected\n");

    //printSymbolTable();

    return 0;
}