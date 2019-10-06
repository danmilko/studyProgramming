#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

enum
{
    PUSH = 1,
    ADD, //the first cmd without arguments
    SUB,
    MULT,
    DIV,
    MOD,
    PRINT,
    DUMP,
    END //always the last cmd
};

const char* CommandNames[] =
{
    "PUSH",
    "ADD",
    "SUB",
    "MULT",
    "DIV",
    "MOD",
    "PRINT",
    "DUMP",
    "END"
};

#endif // COMMANDS_H_INCLUDED
