#ifndef PARSER_ERROR_MESSAGES_H
#define PARSER_ERROR_MESSAGES_H

typedef	enum {
    varNameTooLong = 1, varNameDoesntStartWithLetter = 2, expectedSemicolon = 3,
    expectedIdentifier = 4, expectedPeriod = 5, expectedBecomes = 6, expectedThen = 7,
    expectedDo = 8, expectedEnd = 9, invalidRelationalOperator = 10
} error_type;

// The order of this is with respect to error_type
char *errorMessage[10] = {
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
        "Invalid relational operator detected, possibly because you're a dumbass"

};

#endif //PARSER_ERROR_MESSAGES_H
