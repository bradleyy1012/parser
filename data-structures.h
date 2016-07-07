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
    int intData;
    struct Token *nextToken;
};