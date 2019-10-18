#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#define DEF_CMD(name, length, number_args, code)\
    name,

enum
{
    #include "CommandDefines.h"
    LAST_CMD
};

#undef DEF_CMD



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
