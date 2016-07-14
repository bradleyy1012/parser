void printErrorMessage(int errorCode);
int isValidVariableName(char *name);
void insertIntoSymbolTable(int kind, char *name, int val, int level, int addr);
void haltThatShit();
struct Token* getNextToken();
void addNode(char *c);
void loadTokens();
void printSymbolTable();
void block();
void statement();
void expression();
void condition();
void factor();
void term();
int isRelationalOperator(int c);
int isNumber(int c);
int isLetter(int c);

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

int isValidVariableName(char *name)
{
    if (name == NULL) haltThatShit();
    if (strlen(name) > MAX_IDENTIFIERS_LENGTH) return varNameTooLong;
    if (!isalpha(name[0])) return varNameDoesntStartWithLetter;
    return 0;
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
 * Insert a new symbol into the symbolTable and increment tableIndex.
 * @param the kind of symbol
 * @param name of the symbol
 * @param integer value of symbol
 * @param L level
 * @param M address
 */
void insertIntoSymbolTable(int kind, char *name, int val, int level, int addr)
{
    struct symbol newSymbol;
    newSymbol.kind = kind;
    memcpy(newSymbol.name, name, strlen(name) + 1);
    newSymbol.val = val;
    newSymbol.level = level;
    newSymbol.addr = addr;

    symbolTable[tableIndex] = newSymbol;
    tableIndex++;
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
        haltThatShit();
    }

    // Print out the start of the file
    fprintf(symListFile, "Name\tType\tLevel\tValue\n");

    // Print out errthing else
    for (i = 0; i < tableIndex; i++) {
        switch (symbolTable[i].kind) {
            case var:
                fprintf(symListFile, "%s\tvar\t%d\t%d\n",
                        symbolTable[i].name, symbolTable[i].level, symbolTable[i].val);
                break;
            case constant:
                fprintf(symListFile, "%s\tconst\t0\t%d\n",
                        symbolTable[i].name, symbolTable[i].val);
                break;
            case proc:
                fprintf(symListFile, "%s\tproc\t%d\t1\n",
                        symbolTable[i].name, symbolTable[i].level);
                break;
        }
    }
    fclose(symListFile);
}

/**
 * TODO: Incomplete. Also, find out what this shit actually does
 */
void block()
{
    int result;

    if (currentToken->intData == constsym) {
        printf("constant symbol has been read:  %s\n\n", currentToken->data);
        do {
            getNextToken();
            printf("Next token:  %s\n\n", currentToken->data);
            if (currentToken->intData != identsym) {
                printf("current token != identsym, now halting:  %s\n\n", currentToken->data);
                printErrorMessage(identsym);
                haltThatShit();
            }
            getNextToken();
            if (currentToken->intData != eqsym) {
                //TODO: output error
            }
            getNextToken();

            // If not a number
            if (currentToken->intData == 0) {
                //TODO: output error
            }
            getNextToken();
        } while (currentToken->intData != commasym);

        if (currentToken->intData != semicolonsym) {
            // TODO: output error
        }
        getNextToken();
    }
    if (currentToken->intData == varsym) {
        do {
            getNextToken();
            if (currentToken->intData != identsym) {
                printErrorMessage(expectedIdentifier);
                haltThatShit();
            }

            // After this, currentToken now holds the variable name
            getNextToken();

            printf("Variable name: %s...\n", currentToken->data);

            // isValidVariableName() returns 0 if it is a valid name
            result = isValidVariableName(currentToken->data);
            if (result != 0) {
                printf("Invalid identifier detected!\n");
                printErrorMessage(result);
                haltThatShit();
            }
            insertIntoSymbolTable(var, currentToken->data, 2, 2, 2);

            // After this, currentToken contains either the ',' OR ';' symbol
            getNextToken();

        } while (currentToken->intData == commasym);

        if (currentToken->intData != semicolonsym) {
            printErrorMessage(expectedSemicolon);
            haltThatShit();
        }
        getNextToken();
    }
    while (currentToken->intData == procsym) {
        currentToken = getNextToken();
        if (currentToken->intData != identsym) {
            printErrorMessage(expectedIdentifier);
            haltThatShit();
        }
        currentToken = getNextToken();
        if (currentToken->intData != semicolonsym) {
            printErrorMessage(expectedSemicolon);
            haltThatShit();
        }
        currentToken = getNextToken();
        block();
        if (currentToken->intData != semicolonsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    statement();
}

/**
 * TODO: Incomplete. Also, find out what this shit actually does
 */
void statement()
{
    int codeIndexTemp, codeIndex1, codeIndex2;

    if (currentToken->intData == identsym) {    // TODO: incomplete. not sure if emit should be used.

        // After this, currentToken will be the variable name
        getNextToken();
        printf("Variable name: %s\n", currentToken->data);

        // TODO: make sure the variable was previously declared and shit

        getNextToken();
        if (currentToken->intData != becomessym) {
            printErrorMessage(expectedBecomes);
            haltThatShit();
        }

        getNextToken();
        expression();
    }
    else if (currentToken->intData == callsym) {    // TODO: incomplete. not sure if emit should be used
        currentToken = getNextToken();
        if (currentToken->intData != identsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    else if (currentToken->intData == beginsym) { // TODO: incomplete
        getNextToken();
        statement();

        while (currentToken->intData == semicolonsym) {
            getNextToken();
            statement();
        }
        if (currentToken->intData != endsym) {
            printErrorMessage(expectedEnd);
            haltThatShit();
        }

        // After this, currentToken contains the '.' symbol
        getNextToken();
    }
    else if (currentToken->intData == ifsym) {
        getNextToken();
        condition();
        if (currentToken->intData != thensym) {
            printErrorMessage(expectedThen);
            haltThatShit();
        }
        getNextToken();
        codeIndexTemp = codeIndex;
        emit(JPC, 0, 0);
        statement();
        code[codeIndexTemp].m = codeIndex;
    }
    else if (currentToken->intData == whilesym) {
        codeIndex1 = codeIndex;
        getNextToken();
        condition();
        codeIndex2 = codeIndex;
        emit(JPC, 0, 0); // TODO: find out if this should be gen()
        if (currentToken->intData != dosym) {
            printErrorMessage(expectedDo);
            haltThatShit();
        }
        getNextToken();
        statement();
        emit(JMP, 0, codeIndex1);   // TODO: find out if this should be gen()
        code[codeIndex2].m = codeIndex;
    }
}

/**
 * Handle an expression.
 */
void expression()
{
    int addOp;
    if (currentToken->intData == plussym || currentToken->intData == minussym) {
        addOp = currentToken->intData;
        getNextToken();
        term();
        if (addOp == minussym) {
            emit(OPR, 0, NEG);
        }
    }
    else {
        term();
    }

    // Look-aheads are for pussies
    while ((currentToken->intData == plussym || currentToken->intData == minussym) &&
            currentToken->prevToken->intData != numbersym) {
        printf("Now in while loop in expression(). Value of token: %s\n", currentToken->data);
        addOp = currentToken->intData;
        getNextToken();
        term();
        if (addOp == plussym) {
            emit(OPR, 0, ADD);
        }
        else {
            emit(OPR, 0, SUB);
        }
    }
}

/**
 * TODO: Incomplete. Also, find out what this shit actually does
 */
void condition()
{
    if (currentToken->intData == oddsym) {
        getNextToken();
        expression();
    }
    else {
        expression();
        processRelationalOperator();
        if (!isRelationalOperator(currentToken->intData)) {
            //TODO: output error
        }
        currentToken = getNextToken();
        expression();
    }
}

/**
 * TODO: Incomplete. Also, find out what this shit actually does
 */
void factor()
{
    if (currentToken->intData == identsym) {
        currentToken = getNextToken();
    }
    else if (currentToken->intData == numbersym) {
        getNextToken();
        printf("Value of variable: %s\n", currentToken->data);

        // TODO: do something with the token

        getNextToken();
    }
    else if (currentToken->intData == lparentsym) {
        currentToken = getNextToken();
        expression();
        if (currentToken->intData != rparentsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    else {
        // TODO: output error
    }
}

/**
 * Handle processing a term.
 */
void term()
{
    int multOp;
    factor();
    while (currentToken->intData == multsym || currentToken->intData == slashsym) {
        multOp = currentToken->intData;
        getNextToken();
        factor();
        if (multOp == multsym) {
            emit(OPR, 0, MUL);
        }
        else {
            emit(OPR, 0, DIV);
        }
    }
}

/**
 * @param c the integer value representing the relational operator.
 * @return 1 if c is a relational operator, else 0
 */
int isRelationalOperator(int c)
{
    return c == lessym || c == leqsym || c == gtrsym ||
           c == eqsym || c == geqsym;
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