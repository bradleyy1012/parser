#ifndef PARSER_ERROR_MESSAGES_H
#define PARSER_ERROR_MESSAGES_H

typedef	enum {
    varNameTooLong = 1, varNameDoesntStartWithLetter = 2, expectedSemicolon = 3,
    expectedIdentifier = 4, expectedPeriod = 5, expectedBecomes = 6, expectedThen = 7,
    expectedDo = 8, expectedEnd = 9, invalidRelationalOperator = 10, expectedDigit = 11,
    weirdNullIdentifier = 12, invalidVariableName = 13, expectedRelationalOperator = 14,
    missingIdentSymbol = 15, expectedEqualSymbol = 16, missingNumberSymbol = 17,
    expectedRightParentheses = 18, incompleteFactorDetected = 19, errorOpeningSymbolListFile = 20
} error_type;

// The order of this is with respect to error_type
char *errorMessage[20] = {
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
        "Incomplete factor was encountered. Expected an identifier, number or '('. None was encountered",

        // errorOpeningSymbolListFile
        "There was an error when trying to open symlist.txt"

};

#endif //PARSER_ERROR_MESSAGES_H
