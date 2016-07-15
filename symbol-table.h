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
 * Search for a symbol in the global hash table symbolTable
 * @return the struct symbol if it was found, else NULL
 */
struct symbol *search(char *symName)
{
    int hashIndex = getHashIndex(symName);

    while (symbolTable[hashIndex] != NULL) {
        if (symbolTable[hashIndex]->name == symName) {
            return symbolTable[hashIndex];
        }
        hashIndex++;

        hashIndex %= MAX_SYMBOL_TABLE_SIZE;
    }
    return NULL;
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
    struct symbol *newSymbol = (struct symbol*) malloc(sizeof(struct symbol));
    newSymbol->kind = kind;
    memcpy(newSymbol->name, name, strlen(name) + 1);
    newSymbol->val = val;
    newSymbol->level = level;
    newSymbol->addr = addr;

    int hashIndex = getHashIndex(newSymbol->name);

    while (symbolTable[hashIndex] != NULL) {
        hashIndex++;

        hashIndex %= MAX_SYMBOL_TABLE_SIZE;
    }
    symbolTable[hashIndex] = newSymbol;
}
#endif //PARSER_SYMBOL_TABLE_H
