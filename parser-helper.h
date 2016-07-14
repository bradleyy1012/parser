//
// Created by Bradley Kovacs on 7/14/16.
//

#ifndef PARSER_PARSER_HELPER_H
#define PARSER_PARSER_HELPER_H
void processRelationalOperator()
{
    switch (currentToken->intData) {
        case eqsym: break;  // =
        case neqsym: break; // <>
        case lessym: break; // <
        case leqsym: break; // <=
        case gtrsym: break; // >
        case geqsym: break; // >=
        default: printErrorMessage(invalidRelationalOperator);
            break;
    }
    getNextToken();
}

void processNumber()
{

}

void processIdentifier()
{

}

void processDigit()
{

}

void processLetter()
{

}
#endif //PARSER_PARSER_HELPER_H
