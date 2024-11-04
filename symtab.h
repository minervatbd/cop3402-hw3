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

//check if current scope (Symbol Table) is full/empty
bool isEmpty();
bool isFull();

//pop/push operations for scope stack
void enterScope();
void exitScope();

//add a symbol to the table
void insert(char* tok, SymAttributes el);
//lookup a symbol
SymAttributes* lookup(char* tok);

#endif

 