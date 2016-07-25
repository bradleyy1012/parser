#ifndef PARSER_ERROR_MESSAGES_H
#define PARSER_ERROR_MESSAGES_H

typedef	enum {
    varNameTooLong = 1, varNameDoesntStartWithLetter = 2, expectedSemicolon = 3,
    expectedIdentifier = 4, expectedPeriod = 5, expectedBecomes = 6, expectedThen = 7,
    expectedDo = 8, expectedEnd = 9, invalidRelationalOperator = 10, expectedDigit = 11,
    weirdNullIdentifier = 12, invalidVariableName = 13, expectedRelationalOperator = 14,
    missingIdentSymbol = 15, expectedEqualSymbol = 16, missingNumberSymbol = 17,
    expectedRightParentheses = 18, incompleteFactorDetected = 19, errorOpeningSymbolListFile = 20,
    varWasNeverDeclared = 21, attemptToChangeConstant = 22, invalidVarInExpression = 23,
    identifierNotDeclared = 24, callingConstOrVar = 25, expectedVarAfterRead = 26, expectedIdentAfterWrite = 27
} error_type;

// The order of this is with respect to error_type
char *errorMessage[27] = {
        // varNameTooLong
        "Variable name exceeds the maximum length for variable names",

        // varNameDoesntStartWithLetter
        "Variable name does not start with a letter",

        // expectedSemicolon
        "Expected semicolon, but not encountered",

        // expectedIdentifier
        "Expected identifier, but not encountered",

        // expectedPeriod
        "You forgot a period after 'end'",

        // expectedBecomes
        "Expected ':=' after variable name, but none encountered",

        // expectedThen
        "Expeted 'then' after conditional statement, but not encountered",

        // expectedDo
        "Expected 'do' after condition in while loop, but none encountered",

        // expectedEnd
        "Expected 'end' to complete your code, but not detected",

        // invalidRelationalOperator
        "Invalid relational operator detected",

        // expectedDigit
        "Expected a digit, but got something else instead",

        // weirdNullIdentifier
        "A weird situation occured in which the identifier was read as null",

        // invalidVariableName
        "An invalid variable name was encountered. Variables can only be letters or digits",

        // expectedRelationalOperator
        "Expected a relational operator, but not encountered",

        // missingIdentSymbol
        "Missing ident symbol in lexeme tokens. This was probably caused by the scanner",

        // expectedEqualSymbol
        "Expected '=', but none was encountered",

        // missingNumberSymbol
        "Missing number symbol in lexeme tokens. This was probably caused by the scanner",

        // expectedRightParentheses
        "Expected ')', but none was encountered",

        // incompleteFactorDetected
        "Incomplete factor was encountered. Expected an identifier, number or '(', but none were encountered",

        // errorOpeningSymbolListFile
        "There was an error when trying to open symlist.txt",

        // varWasNeverDeclared
        "You are using an identifier that has not yet been declared",

        // attemptToChangeConstant
        "You can not change the value of a constant",

        // invalidVarInExpression
        "Invalid variable in factor. You can only perform arithmitic with variables, constants or numbers",

        // identifierNotDeclared
        "Your are trying to use an identifier that was never declared",

        // callingConstOrVar
        "You are trying to call something other than a procedure after 'call', which doesn't make sense",

        // expectedVarAfterRead
        "Expected a var after 'read', but none was encountered",

        // expectedIdentAfterWrite
        "Expected an identifer after write, but none was encountered"

};

#endif //PARSER_ERROR_MESSAGES_H
