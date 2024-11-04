#ifndef _SYMTAB_H
#define _SYMTAB_H


#define SCOPE_MAX 999
#define TBL_MAX 999

enum types {VAR, CONST};

typedef struct SymAttributes{
    char* fileName;
    int lineNum;
    enum types type;
    int AR_Offest;
}

typedef struct SymbolTable{
    int levelsOut;
    SymAttributes map[TBL_MAX];
};

typedef struct ScopeStack{
    int top;
    SymbolTable stack[SCOPE_MAX];
};


#endif

 