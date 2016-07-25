//
// Created by Bradley Kovacs on 7/15/16.
//

#ifndef PARSER_SYMBOL_TABLE_H
#define PARSER_SYMBOL_TABLE_H
/**
 * Computes the hash index for the symbol table
 * @param *symName the name of the symbol
 * @return the index of the location for this symbol
 */
int getHashIndex(char *symName)
{
    return (((int) symName[0]) + ((int) symName[strlen(symName) - 1])) % MAX_SYMBOL_TABLE_SIZE;
}

/**
 * Make a new struct symbol and insert it into the symbol table
 * @param kind the enumerated 'kind' of symbol
 * @param name the name of the identifier
 * @param val the integer value
 * @param level the lexicographical level
 * @addr address
 */
void insertIntoSymbolTable(int kind, char *name, int val, int level, int addr)
{
    int hashIndex;
    struct symbol *newSymbol = (struct symbol*) malloc(sizeof(struct symbol));
    newSymbol->kind = kind;
    memcpy(newSymbol->name, name, strlen(name) + 1);
    newSymbol->val = val;
    newSymbol->level = level;
    newSymbol->addr = addr;

    hashIndex = getHashIndex(newSymbol->name);

    while (symbolTable[hashIndex] != NULL) {
        hashIndex++;
        hashIndex %= MAX_SYMBOL_TABLE_SIZE;
    }
    symbolTable[hashIndex] = newSymbol;
}

/**
 * Search for a symbol in the global hash table symbolTable
 * @return the struct symbol if it was found, else NULL
 */
struct symbol *search(char *symName)
{
    int hashIndex = getHashIndex(symName);

    // Since collisions could occur, we just keep checking until a null value is found
    while (symbolTable[hashIndex] != NULL) {
        if (strcmp(symbolTable[hashIndex]->name, symName) == 0) return symbolTable[hashIndex];
        hashIndex++;

        // Loop back to the start if the end is reached
        hashIndex %= MAX_SYMBOL_TABLE_SIZE;
    }
    return NULL;
}

void printHashTable()
{
    int i;
    printf("Now printing everything in the hash table\n");
    for (i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++) {
        if (symbolTable[i] == NULL) continue;
        struct symbol *node = symbolTable[i];
        printf("Found node. Name: %s\nHash index: %d\nValue: %d\n\n", node->name, i, node->val);
    }
}
#endif //PARSER_SYMBOL_TABLE_H
