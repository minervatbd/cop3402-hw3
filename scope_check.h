#ifndef SCOPE_CHECK_H
#define SCOPE_CHECK_H
#include "ast.h"
#include "id_use.h"

// Build the symbol table for the given program AST
// and check the given program AST for duplicate declarations
// or uses of identifiers that were not declared
extern block_t scope_check_program(block_t ast);

// build the symbol table and check the declarations in vds
extern void scope_check_constDecls(const_decls_t cds);

extern void scope_check_constDecl(const_decl_t cd);

extern void scope_check_constIdents(const_def_list_t ids, AST_type t);

extern void scope_check_varDecls(var_decls_t vds);

extern void scope_check_varDecl(var_decl_t vd);

// Add declarations for the names in ids to the current scope's symbol table,
// for variables of the type t,
// producing errors for any duplicate declarations
extern void scope_check_idents(ident_list_t ids, AST_type t);

extern void scope_check_procDecls(proc_decls_t pds);

//extern void scope_check_procDecl(proc_decl_t pd);

//extern void scope_check_procIdents(proc_decl_t pd, AST_type t);

extern void scope_check_declare_procIdent(proc_decl_t id, AST_type t);

// Add a declaration of the name id.name with the type t
// to the current scope's symbol table,
// producing an error if this would be a duplicate declaration
extern void scope_check_declare_ident(ident_t id, AST_type t);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern stmts_t scope_check_stmt(stmts_t stmt);

extern expr_t scope_check_expr(expr_t exp);

extern binary_op_expr_t scope_check_binary_op_expr(binary_op_expr_t exp);

extern ident_t scope_check_ident_expr(ident_t id);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern assign_stmt_t scope_check_assignStmt(assign_stmt_t stmt);

// check the statement to make sure that
// there are no duplicate declarations and that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern call_stmt_t scope_check_callStmt(call_stmt_t stmt);

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
extern if_stmt_t scope_check_ifStmt(if_stmt_t stmt);

extern read_stmt_t scope_check_readStmt(read_stmt_t stmt);

extern print_stmt_t scope_check_printStmt(print_stmt_t stmt);

extern while_stmt_t scope_check_whileStmt(while_stmt_t stmt);

extern block_stmt_t scope_check_blockStmt(block_stmt_t stmt);

// check that the given name has been declared,
// if so, then return an id_use (containing the attributes) for that name,
// otherwise, produce an error using the file_location (floc) given.
// Return the modified AST with id_use pointers
extern id_use *scope_check_ident_declared(file_location floc, const char *name);

#endif
