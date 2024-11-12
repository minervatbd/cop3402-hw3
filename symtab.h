#ifndef _SYMTAB_H
#define _SYMTAB_H

#define SCOPE_MAX 999
#define TBL_MAX 999

enum types {VAR, CONST};

typedef struct{
    int occupied;
    char* fileName;
    int lineNum;
    enum types type;
    int AR_Offest;
} SymAttributes;

typedef struct{
    int levelsIn;
    SymAttributes map[TBL_MAX];
} SymbolTable;

typedef struct{  
    int top;
    SymbolTable stack[SCOPE_MAX];
} ScopeStack;

//check if current scope (Symbol Table) is full/empty
bool isEmpty(ScopeStack* scope);
bool isFull(ScopeStack* scope);

//pop/push operations for scope stack
void enterScope(ScopeStack* scope);
void exitScope(ScopeStack* scope);

//add a symbol to the table
void insert(ScopeStack* scope, char* symb, SymAttributes el);
//lookup a symbol
SymAttributes* lookup(ScopeStack* scope, char* symb);

//hash function
int hash(char* symb);

//constructors
SymbolTable createSymTab(int levelsIn);

#endif

 