To run parser:
    1) Insert the lexeme tokens into lexemelist.txt
    2) In terminal execture: 
        make main
        ./main

Parser features:
    1) Lexeme tokens are stored in a linked list, and each node has a *next and *prev so that, from any node, you can iterate backwards and forwards. This eliminates the need for additional variables, such as look-aheads.
    2) The global variable 'symbolTable' is a hash table. The complexity for insertions is O(1). The complexity for lookups is O(1) for the best case and O(n) for the worst case (which will rarely happen).
    3) The key to the hash table is the symbols name, and the hash index is computed by taking the sum of (the ASCII values of the first and last letter of the key) modulus the maximum symbol table size, which is constant.
