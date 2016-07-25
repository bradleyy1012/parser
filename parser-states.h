//
// Created by Bradley Kovacs on 7/14/16.
//

#ifndef PARSER_PARSER_STATES_H
#define PARSER_PARSER_STATES_H
void processConstantDeclaration();
void processVarDeclaration();
void processRelationalOperator();
void processNumber();
void processIdentifier();
void processProcedureDeclaration();
void block();
void statement();
void condition();
void expression();
void factor();
void term();

/**
 * Simulates the state for const-declaration
 */
void processConstantDeclaration()
{
    if (currentToken->intData == constsym) {
        getNextToken();
        processIdentifier();
        if (currentToken->intData != eqsym) {
            printErrorMessage(expectedEqualSymbol);
            haltThatShit();
        }
        getNextToken();
        processNumber();

        addConstantSymbol(
                currentToken->prevToken->prevToken->prevToken->prevToken->data,
                currentToken->prevToken->prevToken->intData);

        while (currentToken->intData == commasym) {
            getNextToken();
            processIdentifier();
            if (currentToken->intData != eqsym) {
                printErrorMessage(expectedEqualSymbol);
                haltThatShit();
            }
            getNextToken();
            processNumber();

            addConstantSymbol(
                    currentToken->prevToken->prevToken->prevToken->prevToken->data,
                    currentToken->prevToken->prevToken->intData);
        }
        if (currentToken->intData != semicolonsym) {
            printErrorMessage(expectedSemicolon);
            haltThatShit();
        }
        getNextToken();
    }
}

/**
 * Simulates the state for var-declaration
 */
void processVarDeclaration()
{
    if (currentToken->intData == varsym) {
        getNextToken();
        processIdentifier();

        while (currentToken->intData == commasym) {
            getNextToken();
            processIdentifier();
            insertIntoSymbolTable(var, currentToken->prevToken->data, varOffset, lexLevel, 0);
        }
        if (currentToken->intData != semicolonsym) {
            printErrorMessage(expectedSemicolon);
            haltThatShit();
        }
        getNextToken();
    }
}

/**
 * Simulates the state for procedure-declaration
 */
void processProcedureDeclaration()
{
    while (currentToken->intData == procsym) {
        getNextToken();
        processIdentifier();
        addProcedureSymbol(currentToken->prevToken->data);
        if (currentToken->intData != semicolonsym) {
            printErrorMessage(expectedSemicolon);
            haltThatShit();
        }
        getNextToken();
        block();
        if (currentToken->intData != semicolonsym) {
            printErrorMessage(expectedSemicolon);
            haltThatShit();
        }
        getNextToken();
    }
}

/**
 * Simulates the state for block
 */
void block()
{
    lexLevel++;
    processConstantDeclaration();
    processVarDeclaration();
    processProcedureDeclaration();
    statement();
    lexLevel--;
}

/**
 * Simulates the state for rel-op
 */
void processRelationalOperator()
{
    if (!isRelationalOperator(currentToken->intData)) {
        printErrorMessage(expectedRelationalOperator);
        haltThatShit();
    }

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

/**
 * Simulates the state for number
 */
void processNumber()
{
    if (currentToken->intData != numbersym) {
        printErrorMessage(missingNumberSymbol);
        haltThatShit();
    }
    getNextToken();
    int i;
    for (i = 0; i < strlen(currentToken->data); i++) {
        int asciiValue = (int) currentToken->data[i];
        if (asciiValue < 48 || asciiValue > 57) {
            printErrorMessage(expectedDigit);
            haltThatShit();
        }
    }
    getNextToken();
}

/**
 * Simulates the state for ident
 */
void processIdentifier()
{
    if (currentToken->intData != identsym) {
        printErrorMessage(missingIdentSymbol);
        haltThatShit();
    }
    getNextToken();
    verifyValidIdentifierName(currentToken->data);
    getNextToken();
}

/**
 * Simulates the state for expression
 */
void expression()
{
    int addOp;
    if (plusOrMinusSymbolDetected()) {
        addOp = currentToken->intData;
        getNextToken();
        term();
        if (addOp == minussym) emit(OPR, 0, NEG);
    }
    else {
        term();
    }
    while (plusOrMinusSymbolDetected()) {
        addOp = currentToken->intData;
        getNextToken();
        term();

        if (addOp == plussym) emit(OPR, 0, ADD);
        else emit(OPR, 0, SUB);
    }
}

/**
 * Simulates the state for condition
 */
void condition()
{
    int tempSym = currentToken->intData;
    if (tempSym == oddsym) {
        getNextToken();
        expression();
        emit(OPR, 0, ODD);
    }
    else {
        expression();
        processRelationalOperator();
        expression();
        emit(OPR, 0, tempSym);
    }
}

/**
 * Simulates the state for statement
 */
void statement()
{
    struct symbol *t;
    int codeIndexTemp, codeIndex1, codeIndex2, userInput;

    if (currentToken->intData == identsym) {
        processIdentifier();

        t = search(currentToken->prevToken->data);

        // Verify the identifier was previously declared
        if (t == NULL) {
            printErrorMessage(varWasNeverDeclared);
            haltThatShit();
        }

        // Make sure they aren't trying to change the value of a constant
        if (t->kind == constant) {
            printErrorMessage(attemptToChangeConstant);
            haltThatShit();
        }

        if (currentToken->intData != becomessym) {
            printErrorMessage(expectedBecomes);
            haltThatShit();
        }
        getNextToken();
        expression();
    }
    else if (currentToken->intData == callsym) {
        getNextToken();
        processIdentifier();
        t = search(currentToken->prevToken->data);

        if (t == NULL) {
            printErrorMessage(identifierNotDeclared);
            haltThatShit();
        }

        if (t->kind != proc) {
            printErrorMessage(callingConstOrVar);
            haltThatShit();
        }
    }
    else if (currentToken->intData == beginsym) {
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
        emit(JMP, 0, 0);
        code[codeIndexTemp].m = codeIndex;

        if (currentToken->intData == elsesym) {
            getNextToken();
            statement();
        }
    }
    else if (currentToken->intData == whilesym) {
        codeIndex1 = codeIndex;
        getNextToken();
        condition();
        codeIndex2 = codeIndex;
        emit(JPC, 0, 0);
        if (currentToken->intData != dosym) {
            printErrorMessage(expectedDo);
            haltThatShit();
        }
        getNextToken();
        statement();
        emit(JMP, 0, codeIndex1);
        code[codeIndex2].m = codeIndex;
    }
    else if (currentToken->intData == readsym) {
        getNextToken();
        processIdentifier();

        t = search(currentToken->prevToken->data);

        // Since you can only read in a var
        if (t->kind != var) {
            printErrorMessage(expectedVarAfterRead);
            haltThatShit();
        }

        printf("Enter a value for %s: ", currentToken->prevToken->data);
        scanf("%d", &userInput);

        t = search(currentToken->prevToken->data);
        if (t == NULL) {
            printErrorMessage(varWasNeverDeclared);
            haltThatShit();
        }
        t->val = userInput;
    }
    else if (currentToken->intData == writesym) {
        getNextToken();
        if (currentToken->intData != identsym) {
            printErrorMessage(expectedIdentAfterWrite);
            haltThatShit();
        }
        getNextToken();
        verifyValidIdentifierName(currentToken->data);
        getNextToken();

        t = search(currentToken->prevToken->data);
        if (t == NULL) {
            printErrorMessage(varWasNeverDeclared);
            haltThatShit();
        }
        printf("%d\n", t->val);
    }
}

/**
 * Simulates the state for factor
 */
void factor()
{
    if (currentToken->intData == identsym) {
        processIdentifier();
        struct symbol *t = search(currentToken->prevToken->data);

        if (t == NULL) {
            printErrorMessage(varWasNeverDeclared);
            haltThatShit();
        }

        if (!(t->kind == var || t->kind == constant)) {
            printErrorMessage(invalidVarInExpression);
            haltThatShit();
        }
    }
    else if (currentToken->intData == numbersym) {
        processNumber();
    }
    else if (currentToken->intData == lparentsym) {
        getNextToken();
        expression();
        if (currentToken->intData != rparentsym) {
            printErrorMessage(expectedRightParentheses);
            haltThatShit();
        }
        getNextToken();
    }
    else {
        printErrorMessage(incompleteFactorDetected);
        haltThatShit();
    }
}

/**
 * Simulates the state for term
 */
void term()
{
    int multOp;
    factor();
    while (multOrDivideSymbolDetected()) {
        multOp = currentToken->intData;
        getNextToken();
        factor();
        if (multOp == multsym) emit(OPR, 0, MUL);
        else emit(OPR, 0, DIV);
    }
}
#endif //PARSER_PARSER_STATES_H
