#define MAX_SYMBOL_TABLE_SIZE 100

// Global variables
FILE* lexemeListFile;
int token;

typedef	enum {
    nulsym	=	1,	identsym,	numbersym,	plussym,	minussym,
    multsym,		slashsym,	oddsym,	eqsym,	neqsym,	lessym,	leqsym,
    gtrsym,	geqsym,	lparentsym,	rparentsym,	commasym,	semicolonsym,
    periodsym,	becomessym,	beginsym,	endsym,	ifsym,	thensym,
    whilesym,	dosym,	callsym,	constsym,	varsym,	procsym,	writesym,
    readsym	,	elsesym
} token_type;

typedef struct symbol {
    int kind;       // const = 1, var = 2, proc = 3
    char name[12];  // name up to 11 chars
    int val;        // number (ASCII value)
    int level;      // L level
    int addr;       // M address
} symbol;

symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

void expression();
void factor();
void term();
int isRelationalOperator(int c);
void condition();
void statement();
void block();
int getToken();

int getToken()
{
    int c = fgetc(lexemeListFile);
    return c;
}

void expression()
{
    if (token == plussym || token == minussym) {
        token = getToken();
    }
    term();
    while (token == plussym || token == minussym) {
        token = getToken();
        term();
    }
}

void factor()
{
    if (token == identsym) {
        token = getToken();
    }
    else if (isdigit(token)) {
        token = getToken();
    }
    else if (token == lparentsym) {
        token = getToken();
        expression();
        if (token != rparentsym) {
            // TODO: output error
        }
        token = getToken();
    }
    else {
        // TODO: output error
    }
}

void term()
{
    factor();
    while (token == multsym || token == slashsym) {
        token = getToken();
        factor();
    }
}

int isRelationalOperator(int c)
{
    return c == lessym || c == leqsym || c == gtrsym ||
           c == eqsym || c == geqsym;
}

void condition()
{
    if (token == oddsym) {
        token = getToken();
        expression();
    }
    else {
        expression();
        if (!isRelationalOperator(token)) {
            //TODO: output error
        }
        token = getToken();
        expression();
    }
}

void statement()
{
    if (token == identsym) {
        token = getToken();
        if (token != becomessym) {
            // TODO: output error
        }
        token = getToken();
        expression();
    }
    else if (token == callsym) {
        token = getToken();
        if (token != identsym) {
            // TODO: output error
        }
        token = getToken();
    }
    else if (token == beginsym) {
        token = getToken();
        statement();
        while (token == semicolonsym) {
            token = getToken();
            statement();
        }
        if (token != endsym) {
            // TODO: output error
        }
        token = getToken();
    }
    else if (token == ifsym) {
        token = getToken();
        condition();
        if (token != thensym) {
            // TODO: output error
        }
        token = getToken();
        statement();
    }
    else if (token == whilesym) {
        token = getToken();
        condition();
        if (token != dosym) {
            // TODO: output error
        }
        token = getToken();
        statement();
    }
}

void block()
{
    if (token == constsym) {
        do {
            token = getToken();
            if (token != identsym) {
                //TODO: output error
            }
            token = getToken();
            if (token != eqsym) {
                //TODO: output error
            }
            token = getToken();
            if (!isdigit(token)) {
                //TODO: output error
            }
            token = getToken();
        } while (token != commasym);

        if (token != semicolonsym) {
            // TODO: output error
        }
        token = getToken();
    }
    if (token == varsym) {
        do {
            token = getToken();
            if (token != identsym) {
                // TODO: output error
            }
            token = getToken();
        } while (token != commasym);

        if (token != semicolonsym) {
            // TODO: output error
        }
        token = getToken();
    }
    while (token == procsym) {
        token = getToken();
        if (token != identsym) {
            // TODO: output error
        }
        token = getToken();
        if (token != semicolonsym) {
            // TODO: output error
        }
        token = getToken();
        block();
        if (token != semicolonsym) {
            // TODO: output error
        }
        token = getToken();
    }
    statement();
}