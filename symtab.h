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
bool isEmpty(ScopeStack* stack);
bool isFull(ScopeStack* stack);

//pop/push operations for scope stack
void enterScope(ScopeStack* stack);
void exitScope(ScopeStack* stack);

//add a symbol to the table
void insert(ScopeStack* stack, char* tok, SymAttributes el);
//lookup a symbol
SymAttributes* lookup(ScopeStack* stack, char* tok);

//hash function
int hash(char* tok);

#endif

 