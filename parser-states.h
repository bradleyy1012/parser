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
                currentToken->prevToken->prevToken->intData,
                0
        );

        printf("Constant name: %s\nValue: %s\n",
               currentToken->prevToken->prevToken->prevToken->prevToken->data,
               currentToken->prevToken->prevToken->data);

        while (currentToken->intData == commasym) {
            getNextToken();
            processIdentifier();
            if (currentToken->intData != eqsym) {
                printErrorMessage(expectedEqualSymbol);
                haltThatShit();
            }
            getNextToken();
            processNumber();
        }
        if (currentToken->intData != semicolonsym) {
            printErrorMessage(expectedSemicolon);
            haltThatShit();
        }
        getNextToken();
    }
}

/**
 * Status: COMPLETE
 * Simulates the state for var-declaration
 */
void processVarDeclaration()
{
    if (currentToken->intData == varsym) {
        printf("Varsym processed!\n");
        getNextToken();
        processIdentifier();

        addVariableSymbol(currentToken->prevToken->data, 0, 0);

        while (currentToken->intData == commasym) {
            getNextToken();
            processIdentifier();
            addVariableSymbol(currentToken->prevToken->data, 0, 0);
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
 * Status: COMPLETE
 * Simulates the state for rel-op
 */
void processRelationalOperator()
{
    // Make sure it's a rel-op
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
 * Status: COMPLETE
 * Simulates the state for number
 */
void processNumber()
{
    if (currentToken->intData != numbersym) {
        printErrorMessage(missingNumberSymbol);
        haltThatShit();
    }
    getNextToken();
    printf("Value of number: %s\n", currentToken->data);

    // TODO: after project is completed, use the function to just test the entire string for ints

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
 * Status: COMPLETE
 * Simulates the state for ident
 */
void processIdentifier()
{
    if (currentToken->intData != identsym) {
        printErrorMessage(missingIdentSymbol);
        haltThatShit();
    }
    getNextToken();
    printf("Identifier name: %s\n", currentToken->data);
    // This should never happen, but just in case
    if (currentToken->data == NULL) {
        printErrorMessage(weirdNullIdentifier);
        haltThatShit();
    }

    // Verify correct length
    int len = strlen(currentToken->data);
    if (len > MAX_IDENTIFIERS_LENGTH) {
        printErrorMessage(varNameTooLong);
        haltThatShit();
    }

    // Verify it starts with a letter
    if (!isalpha(currentToken->data[0])) {
        printErrorMessage(varNameDoesntStartWithLetter);
        haltThatShit();
    }

    // Iterate in reverse to save memory space
    for (; len > 0; len--) {
        // Verify the len-th character is a letter or digit
        if (!isalnum(currentToken->data[len - 1])) {
            printErrorMessage(invalidVariableName);
            haltThatShit();
        }
    }

    getNextToken();
}

/**
 * Handle an expression.
 */
void expression()
{
    int addOp;
    if (plusOrMinusSymbolDetected()) {
        addOp = currentToken->intData;
        getNextToken();
        term();
        /*
        if (addOp == minussym) {
            emit(OPR, 0, NEG);
        }
         */
    }
    else {
        term();
    }

    while (plusOrMinusSymbolDetected()) {
        printf("Plus (%d) or minus (%d) symbol detected! Value of token: %d\n",
               plussym, minussym, currentToken->intData);
        addOp = currentToken->intData;
        getNextToken();
        term();

        /*
        if (addOp == plussym) {
            emit(OPR, 0, ADD);
        }
        else {
            emit(OPR, 0, SUB);
        }
         */
    }
}

/**
 * Simulates the state for condition
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
        expression();
    }
}

/**
 * Simulates the state for statement
 */
void statement()
{
    int codeIndexTemp, codeIndex1, codeIndex2, val;

    if (currentToken->intData == identsym) {
        processIdentifier();
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
        //codeIndexTemp = codeIndex;
        //emit(JPC, 0, 0);
        statement();
        //code[codeIndexTemp].m = codeIndex;

        if (currentToken->intData == elsesym) {
            getNextToken();
            statement();
        }
    }
    else if (currentToken->intData == whilesym) {
        getNextToken();
        codeIndex1 = codeIndex;
        condition();
        codeIndex2 = codeIndex;
        //emit(JPC, 0, 0); // TODO: find out if this should be gen()
        if (currentToken->intData != dosym) {
            printErrorMessage(expectedDo);
            haltThatShit();
        }
        getNextToken();
        statement();
        //emit(JMP, 0, codeIndex1);   // TODO: find out if this should be gen()
        //code[codeIndex2].m = codeIndex;
    }
    else if (currentToken->intData == readsym) {
        getNextToken();
        processIdentifier();
        printf("Enter a value for %s: ", currentToken->prevToken->data);
        scanf("%d", &val);  // TODO: store this value somewhere
    }
    else if (currentToken->intData == writesym) {
        getNextToken();
        processIdentifier();
        printf("%s = \n", currentToken->prevToken->data);
    }
}

/**
 * Simulates the state for factor
 */
void factor()
{
    if (currentToken->intData == identsym) {
        processIdentifier();
    }
    else if (currentToken->intData == numbersym) {
        printf("Number symbol detected\n");
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
        printf("HERE: %s\n", currentToken->data);
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
        if (multOp == multsym) {
            //emit(OPR, 0, MUL);
        }
        else {
            //emit(OPR, 0, DIV);
        }
    }
}
#endif //PARSER_PARSER_STATES_H
