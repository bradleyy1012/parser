typedef	enum {
    nulsym = 1,	identsym,	numbersym,	plussym,	minussym,
    multsym,		slashsym,	oddsym,	eqsym,	neqsym,	lessym,	leqsym,
    gtrsym,	geqsym,	lparentsym,	rparentsym,	commasym,	semicolonsym,
    periodsym,	becomessym,	beginsym,	endsym,	ifsym,	thensym,
    whilesym,	dosym,	callsym,	constsym,	varsym,	procsym,	writesym,
    readsym	,	elsesym
} token_type;

typedef enum {
    constant = 1, var = 2, proc = 3
} kind;

typedef struct symbol {
    int kind;       // const = 1, var = 2, proc = 3
    char name[12];  // name up to 11 chars
    int val;        // number (ASCII value)
    int level;      // L level
    int addr;       // M address
} symbol;

struct Token {
    char *data;
    struct Token *nextToken;
};

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

void expression();
void factor();
void term();
int isRelationalOperator(int c);
void condition();
void statement();
void block();
void printSymbolTable();
void loadTokens();
int isNumber(int c);
int isLetter(int c);
void addNode(char *c);
struct Token* getNextToken();

struct Token* getNextToken()
{
    if (tokenNodeHead == NULL) {
        exit(EXIT_FAILURE);
    }
    struct Token *token = tokenNodeHead;
    tokenNodeHead = tokenNodeHead->nextToken;
    return token;
}

void addNode(char *c)
{
    if(tokenNodeHead == NULL) {
        tokenNodeHead = (struct Token*)malloc(sizeof(struct Token));
        tokenNodeHead->data = c;
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
    }
}

void loadTokens()
{
    int c, idx;
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
            char *token = (char*) malloc((idx + 1) * sizeof(char));
            memcpy(token, currentToken, idx);
            addNode(token);
        }
    }
    fclose(lexemeListFile);
}

void printSymbolTable()
{
    FILE* symListFile = fopen("symlist.txt", "w");
    int i;

    fprintf(symListFile, "Name\tType\tLevel\tValue\n");
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

void block()
{
    if (atoi(currentToken->data) == constsym) {
        do {
            currentToken = getNextToken();
            if (atoi(currentToken->data) != identsym) {
                //TODO: output error
            }
            currentToken = getNextToken();
            if (atoi(currentToken->data) != eqsym) {
                //TODO: output error
            }
            currentToken = getNextToken();

            // If not a digit
            if (atoi(currentToken->data) == 0) {
                //TODO: output error
            }
            currentToken = getNextToken();
        } while (atoi(currentToken->data) != commasym);

        if (atoi(currentToken->data) != semicolonsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    if (atoi(currentToken->data) == varsym) {
        do {
            currentToken = getNextToken();
            if (atoi(currentToken->data) != identsym) {
                // TODO: output error
            }
            currentToken = getNextToken();
        } while (atoi(currentToken->data) != commasym);

        if (atoi(currentToken->data) != semicolonsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    while (atoi(currentToken->data) == procsym) {
        currentToken = getNextToken();
        if (atoi(currentToken->data) != identsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        if (atoi(currentToken->data) != semicolonsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        block();
        if (atoi(currentToken->data) != semicolonsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    statement();
}

void statement()
{
    if (atoi(currentToken->data) == identsym) {
        currentToken = getNextToken();
        if (atoi(currentToken->data) != becomessym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        expression();
    }
    else if (atoi(currentToken->data) == callsym) {
        currentToken = getNextToken();
        if (atoi(currentToken->data) != identsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    else if (atoi(currentToken->data) == beginsym) {
        currentToken = getNextToken();
        statement();
        while (atoi(currentToken->data) == semicolonsym) {
            currentToken = getNextToken();
            statement();
        }
        if (atoi(currentToken->data) != endsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    else if (atoi(currentToken->data) == ifsym) {
        currentToken = getNextToken();
        condition();
        if (atoi(currentToken->data) != thensym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        statement();
    }
    else if (atoi(currentToken->data) == whilesym) {
        currentToken = getNextToken();
        condition();
        if (atoi(currentToken->data) != dosym) {
            // TODO: output error
        }
        currentToken = getNextToken();
        statement();
    }
}

void expression()
{
    if (atoi(currentToken->data) == plussym || atoi(currentToken->data) == minussym) {
        currentToken = getNextToken();
    }
    term();
    while (atoi(currentToken->data) == plussym || atoi(currentToken->data) == minussym) {
        currentToken = getNextToken();
        term();
    }
}

void condition()
{
    if (atoi(currentToken->data) == oddsym) {
        currentToken = getNextToken();
        expression();
    }
    else {
        expression();
        if (!isRelationalOperator(atoi(currentToken->data))) {
            //TODO: output error
        }
        currentToken = getNextToken();
        expression();
    }
}

void factor()
{
    if (atoi(currentToken->data) == identsym) {
        currentToken = getNextToken();
    }
    else if (isdigit(atoi(currentToken->data))) {  // TODO: This could possibly cause an error
        currentToken = getNextToken();
    }
    else if (atoi(currentToken->data) == lparentsym) {
        currentToken = getNextToken();
        expression();
        if (atoi(currentToken->data) != rparentsym) {
            // TODO: output error
        }
        currentToken = getNextToken();
    }
    else {
        // TODO: output error
    }
}

void term()
{
    factor();
    while (atoi(currentToken->data) == multsym || atoi(currentToken->data) == slashsym) {
        currentToken = getNextToken();
        factor();
    }
}

int isRelationalOperator(int c)
{
    return c == lessym || c == leqsym || c == gtrsym ||
           c == eqsym || c == geqsym;
}

int isNumber(int c)
{
    return c >= 48 && c <= 57;
}

int isLetter(int c)
{
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}