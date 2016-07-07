/*
 * Created by Bradley Kovacs
 * COP 3402 - Summer 2016
 * Assignment 3 - Parser
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "data-structures.h"

#define MAX_SYMBOL_TABLE_SIZE 100
#define MAX_IDENTIFIERS_LENGTH 11

// Global variables
FILE* lexemeListFile;
int tableIndex, token;
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

// currentToken is the current lexeme being processed
struct Token *tokenNodeHead, *currentToken;

#include "parser.h"

int main(int argc, const char*argv[])
{
    tokenNodeHead = NULL;
    loadTokens();

    struct Token *node = tokenNodeHead;
    while (node != NULL) {
        printf("Integer value: %d\n", node->intData);
        node = node->nextToken;
    }

    // Initially set the table index
    tableIndex = 0;

    // Get the token
    currentToken = getNextToken();
    block();

    //printSymbolTable();

    return 0;
}