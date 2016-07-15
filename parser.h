void addConstantSymbol(char *name, int val, int address);
void addVariableSymbol(char *name, int val, int address);
void addProcedureSymbol(char *name, int val, int address);
void addNode(char *c);
void loadTokens();
void printSymbolTable();
void expression();
void condition();
void factor();
void term();
int isNumber(int c);
int isLetter(int c);
int isRelationalOperator(int tok);
int plusOrMinusSymbolDetected();
int multOrDivideSymbolDetected();
void addConstantSymbol();

/**
 * Add a constant symbol to the symbol table
 */
void addConstantSymbol(char *name, int val, int address)
{
    insertIntoSymbolTable(constant, name, val, lexLevel, address);
}

/**
 * Add a variable to the symbol table
 */
void addVariableSymbol(char *name, int val, int address)
{
    insertIntoSymbolTable(var, name, val, lexLevel, address);
}

/**
 * Add a procedure to the symbol table
 */
void addProcedureSymbol(char *name, int val, int address)
{
    insertIntoSymbolTable(proc, name, val, lexLevel, address);
}

/**
 * Prints an error message
 * @param errorCode the error code with respect to error_type
 */
void printErrorMessage(int errorCode)
{
    printf("The last lexeme is %s\n", currentToken->data);
    int numTokensProcessed = numTokensRead - 1;
    float percentComplete;
    struct Token *t = tokenNodeHead;
    while (t != NULL) {
        numTokensProcessed--;
        t = t->nextToken;
    }
    percentComplete = (numTokensProcessed * 100) / numTokensRead;
    printf("Error %2.1f%% through the program\n%s\n",
           percentComplete, errorMessage[errorCode - 1]);
}

/**
 * Insert a new instruction into the code[] array and increment codeIndex
 * @param op the operation code
 * @param l the lexicographic level
 * @param m the modifier
 */
void emit(int op, int l, int m)
{
    if (codeIndex > MAX_CODE_LENGTH) {
        // TODO: print error message
    }
    struct instruction *newInstruction;
    newInstruction->op = op;
    newInstruction->l = l;
    newInstruction->m = m;

    code[codeIndex++] = *newInstruction;
}

/**
 * Stop execution of the program.
 */
void haltThatShit()
{
    printf("Compiling process has failed and is now stopping. Fix your shit and try again\n");
    exit(EXIT_FAILURE);
}

/**
 * Get the next token in the linked list, starting with
 * the head node tokenNodeHead.
 * @return struct Token* the next token to be processed
 */
struct Token* getNextToken()
{
    if (tokenNodeHead == NULL) {
        return NULL;
    }

    currentToken = tokenNodeHead;
    tokenNodeHead = tokenNodeHead->nextToken;
    return currentToken;
}

/**
 * Add a node to the linked list data structure. The head
 * node is tokenNodeHead.
 * @param *c the string to be assigned to the new nodes value
 */
void addNode(char *c)
{
    if(tokenNodeHead == NULL) {
        tokenNodeHead = (struct Token*)malloc(sizeof(struct Token));
        tokenNodeHead->data = c;
        tokenNodeHead->intData = atoi(c);
        tokenNodeHead->nextToken = NULL;
        tokenNodeHead->prevToken = NULL;
    }
    else {
        //Search for the next available space in the linked list
        struct Token *newNode = tokenNodeHead;
        while(newNode->nextToken != NULL) {
            newNode = newNode->nextToken;
        }

        // Make room for this fucker and assign it a new address in memory
        newNode->nextToken = (struct Token*)malloc(sizeof(struct Token));

        // Keep track of the current node, which allows me to backtrack the fuck out of anything
        struct Token *previous = newNode;

        newNode = newNode->nextToken;
        newNode->nextToken = NULL;

        newNode->prevToken = previous;
        newNode->data = c;
        newNode->intData = atoi(c);
    }
}

/**
 * Read in the input from lexemelist.txt and load the tokens
 * into a linked list. The head node is tokenNodeHead.
 */
void loadTokens()
{
    int c, idx, count = 0;
    char *token;
    FILE* lexemeListFile = fopen("lexemelist.txt", "r");

    printf("Lexeme tokens: ");
    while ((c = fgetc(lexemeListFile)) != EOF) {
        // Make sure we don't read a space
        if (isNumber(c) || isLetter(c)) {

            idx = 0;

            // TODO: this causes an Abort trap 6
            char currentToken[MAX_IDENTIFIERS_LENGTH];

            while (isNumber(c) || isLetter(c)) {
                currentToken[idx] = toascii(c);
                c = fgetc(lexemeListFile);
                idx++;
            }
            token = (char*) malloc((idx + 1) * sizeof(char));
            memcpy(token, currentToken, idx);
            addNode(token);
            count++;
            printf("%s ", token);
        }
    }
    fclose(lexemeListFile);
    printf("\n");
    numTokensRead = count;
}

/**
 * Print the symbols from symbolTable[] to symlist.txt.
 */
void printSymbolTable()
{
    int i;
    FILE* symListFile = fopen("symlist.txt", "w");

    if (symListFile == NULL) {
        printErrorMessage(errorOpeningSymbolListFile);
        haltThatShit();
    }

    // Print out the start of the file
    fprintf(symListFile, "Name\tType\tLevel\tValue\n");

    // Print out errthing else
    for (i = 0; i < MAX_SYMBOL_TABLE_SIZE; i++) {

        // No point in processing shit thats not there
        if (symbolTable[i] == NULL) continue;

        switch (symbolTable[i]->kind) {
            case var:
                fprintf(symListFile, "%4s\tvar \t%4d\t%5d\n",
                        symbolTable[i]->name, symbolTable[i]->level, symbolTable[i]->val);
                break;
            case constant:
                fprintf(symListFile, "%4s\tconst\t%4d\t%5d\n",
                        symbolTable[i]->name, symbolTable[i]->level, symbolTable[i]->val);
                break;
            case proc:
                fprintf(symListFile, "%4s\tproc\t%4d\t1\n",
                        symbolTable[i]->name, symbolTable[i]->level);
                break;
        }
    }
    fclose(symListFile);
}

/**
 * Determines if an ASCII value relates to an upper-case or
 * lower-case letter.
 * @param c the ASCII integer value
 * @return 1 if the ASCII value is a letter, else 0
 */
int isNumber(int c)
{
    return c >= 48 && c <= 57;
}

/**
 * Determines if an ASCII value relates to a number.
 * @param c the ASCII integer value
 * @return 1 if the ASCII value is an integer, else 0
 */
int isLetter(int c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

/**
 * @param c the integer value representing the relational operator.
 * @return 1 if c is a relational operator, else 0
 */
int isRelationalOperator(int tok)
{
    return tok == eqsym || tok == neqsym || tok == lessym || tok == leqsym ||
           tok == gtrsym || tok == geqsym;
}

/**
 * Decides whether a plus or minus symbol was encountered in the currentToken
 * @return 1 if plussym or minussym was detected
 */
int plusOrMinusSymbolDetected()
{
    return (currentToken->intData == plussym || currentToken->intData == minussym) &&
           currentToken->prevToken->intData != numbersym;
}

/**
 * Decides whether a multiplication or division symbol was encountered in the currentToken
 * @return 1 if multsym or slashsym was detected
 */
int multOrDivideSymbolDetected()
{
    return (currentToken->intData == multsym || currentToken->intData == slashsym) &&
           currentToken->prevToken->intData != numbersym;
}
