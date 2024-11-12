#include "parser_types.h"
#include "symtab.h"

bool isEmpty(ScopeStack* scope){
    return scope->top == 0 ? 1 : 0;
}

bool isFull(ScopeStack* scope){
    return scope->top == SCOPE_MAX ? 1 : 0;
}

void enterScope(ScopeStack* scope){
    int new = scope->top + 1;
    scope->stack[new] = createSymTab(scope->stack[scope->top].levelsIn); 
    scope->top = new;
}

void exitScope(ScopeStack* scope){
    scope->top = scope->top - 1;
}

void insert(ScopeStack* scope, char* symb, SymAttributes el){
    int index = hash(symb);
    int tbd = 0;

    while(scope->stack[scope->top].map[index].occupied == 1)
        index ++;

    scope->stack[scope->top].map[index].AR_Offest = tbd;
    scope->stack[scope->top].map[index].fileName = "xys";
    scope->stack[scope->top].map[index].lineNum = tbd;
    scope->stack[scope->top].map[index].type = tbd;
}

SymAttributes lookup(ScopeStack* scope, char* symb){

}

int hash(char* symb){
    return symb[0] % 26 * symb[1]; 
}

SymbolTable createSymTab(int levelsIn){
    SymbolTable tab;
    tab.levelsIn = levelsIn;
    
    for(int i=0; i<TBL_MAX; i++)
        tab.map[i].occupied = 0;

    return tab;    
}

