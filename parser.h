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

/**
 * Stop execution of the program.
 */
void haltThatShit()
{
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
        haltThatShit();
    }
    struct Token *token = tokenNodeHead;
    tokenNodeHead = tokenNodeHead->nextToken;
    return token;
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
    }
    else {
        //Search for the next available space in the linked list
        struct Token *newNode = tokenNodeHead;
        while(newNode->nextToken != NULL) {
            newNode = newNode->nextToken;
        }
        newNode->nextToken = (struct Token*)malloc(sizeof(struct Token));
        newNode = newNode->nextToken;
        newNode->nextToken = NULL;
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
    int c, idx;
    char *token;
    lexemeListFile = fopen("lexemelist.txt", "r");

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
        }
    }
    fclose(lexemeListFile);
}

/**
 * Print the symbols from symbol_table[] to symlist.txt.
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
        switch (symbol_table[i].kind) {
            case var:
                fprintf(symListFile, "%s\tvar\t%d\t%d\n",
                        symbol_table[i].name, symbol_table[i].level, symbol_table[i].val);
                break;
            case constant:
                fprintf(symListFile, "%s\tconst\t0\t%d\n", symbol_table[i].name, symbol_table[i].val);
                break;
            case proc:
                fprintf(symListFile, "%s\tproc\t%d\t1\n", symbol_table[i].name, symbol_table[i].level);
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
    if (currentToken->intData == constsym) {
        do {
            getNextToken();
            if (currentToken->intData != identsym) {
                //TODO: output error
            }
            currentToken = getNextToken();
            if (currentToken->intData != eqsym) {
                //TODO: output error
            }
            currentToken = getNextToken();

            // If not a digit
            if (currentToken->intData == 0) {
                //TODO: output error
            }
            currentToken = getNextToken();
        } while (currentToken->intData != commasym);

        if (currentToken->intData != semicolonsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    if (currentToken->intData == varsym) {
        do {
            currentToken = getNextToken();
            if (currentToken->intData != identsym) {
                // TODO: output error
            }
            currentToken = getNextToken();
        } while (currentToken->intData != commasym);

        if (currentToken->intData != semicolonsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    while (currentToken->intData == procsym) {
        currentToken = getNextToken();
        if (currentToken->intData != identsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        if (currentToken->intData != semicolonsym) {
            // TODO: output error
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
    if (currentToken->intData == identsym) {
        currentToken = getNextToken();
        if (currentToken->intData != becomessym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        expression();
    }
    else if (currentToken->intData == callsym) {
        currentToken = getNextToken();
        if (currentToken->intData != identsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    else if (currentToken->intData == beginsym) {
        currentToken = getNextToken();
        statement();
        while (currentToken->intData == semicolonsym) {
            currentToken = getNextToken();
            statement();
        }
        if (currentToken->intData != endsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    else if (currentToken->intData == ifsym) {
        currentToken = getNextToken();
        condition();
        if (currentToken->intData != thensym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        statement();
    }
    else if (currentToken->intData == whilesym) {
        currentToken = getNextToken();
        condition();
        if (currentToken->intData != dosym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        statement();
    }
}

/**
 * TODO: Incomplete. Also, find out what this shit actually does
 */
void expression()
{
    if (currentToken->intData == plussym || currentToken->intData == minussym) {
        currentToken = getNextToken();
    }
    term();
    while (currentToken->intData == plussym || currentToken->intData == minussym) {
        currentToken = getNextToken();
        term();
    }
}

/**
 * TODO: Incomplete. Also, find out what this shit actually does
 */
void condition()
{
    if (currentToken->intData == oddsym) {
        currentToken = getNextToken();
        expression();
    }
    else {
        expression();
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
    else if (isdigit(currentToken->intData)) {  // TODO: This could possibly cause an error
        currentToken = getNextToken();
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
 * TODO: Incomplete. Also, find out what this shit actually does
 */
void term()
{
    factor();
    while (currentToken->intData == multsym || currentToken->intData == slashsym) {
        currentToken = getNextToken();
        factor();
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