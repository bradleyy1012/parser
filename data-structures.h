#ifndef PARSER_DATA_STRUCTURES_H
#define PARSER_DATA_STRUCTURES_H

typedef	enum {
    nulsym = 1,	    identsym = 2,   numbersym = 3,  plussym = 4,    minussym = 5,   multsym = 6,
    slashsym = 7,   oddsym = 8,     eqsym = 9,	    neqsym = 10,    lessym = 11,    leqsym = 12,
    gtrsym = 13,	geqsym = 14,	lparentsym = 15,
    rparentsym = 16, commasym = 17, semicolonsym = 18,
    periodsym = 19,	becomessym = 20,
    beginsym = 21,	endsym = 22,	ifsym = 23,	    thensym = 24,   whilesym = 25,  dosym = 26,
    callsym = 27,	constsym = 28,	varsym = 29,	procsym = 30,	writesym = 31,
    readsym	= 32,	elsesym = 33
} token_type;

typedef enum {
    constant = 1, var = 2, proc = 3
} kind;

typedef enum {
    LIT = 1, OPR, LOD, STO, CAL, INC, JMP, JPC, SIO1, SIO2, SIO3
} OP_CODE;

typedef enum {
    RET = 0, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ
} ALU_CODE;

typedef struct instruction {
    int op; // opcode
    int l;  // L
    int m;  // M
} instruction;

struct Token {
    char *data;
    int intData;
    struct Token *nextToken;
    struct Token *prevToken;
};

typedef struct symbol {
    int kind;       // const = 1, var = 2, proc = 3
    char name[12];  // name up to 11 chars (also the key for hash table)
    int val;        // number (ASCII value)
    int level;      // L level
    int addr;       // M address
} symbol;
#endif //PARSER_DATA_STRUCTURES_H