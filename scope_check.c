#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scope_check.h"
#include "id_attrs.h"
#include "file_location.h"
#include "ast.h"
#include "utilities.h"
#include "symtab.h"
#include "scope_check.h"

// Build the symbol table for prog
// and check for duplicate declarations
// or uses of undeclared identifiers
// Return the modified AST with id_use pointers
block_t scope_check_program(block_t ast)
{
    symtab_enter_scope();
    scope_check_constDecls(ast.const_decls);
    scope_check_varDecls(ast.var_decls);
    scope_check_procDecls(ast.proc_decls);
    // need to update stmt's AST with id_use structs
    ast.stmts = scope_check_stmt(ast.stmts);
    symtab_leave_scope();
    return ast;
}

// build the symbol table and check the declarations in vds
void scope_check_varDecls(var_decls_t vds)
{
    var_decl_t *vdp = vds.var_decls;
    while (vdp != NULL) {
        scope_check_varDecl(*vdp);
        vdp = vdp->next;
    }
}

// Add declarations for the names in vd,
// reporting duplicate declarations
void scope_check_varDecl(var_decl_t vd)
{
    id_kind type = 1;
    scope_check_idents(vd.ident_list, type);
}

// Add declarations for the names in ids
// to current scope as type t
// reporting any duplicate declarations
void scope_check_idents(ident_list_t ids, id_kind t)
{
    ident_t *idp = ids.start;
    while (idp != NULL) {
        scope_check_declare_ident(*idp, t);
        idp = idp->next;
    }
}

// Add declaration for id
// to current scope as type t
// reporting if it's a duplicate declaration
void scope_check_declare_ident(ident_t id, id_kind t)
{
    if (symtab_declared_in_current_scope(id.name)) {
        id_use *test = symtab_lookup(id.name);
        if(test->attrs->kind == constant_idk) {
            bail_with_prog_error(*(id.file_loc), "variable \"%s\" is already declared as a constant", id.name);
        }
        else{
            bail_with_prog_error(*(id.file_loc), "variable \"%s\" is already declared as a variable", id.name);
        }     
    } 
    else {
        int ofst_cnt = symtab_scope_loc_count();
        id_attrs *attrs = create_id_attrs(*(id.file_loc), t, ofst_cnt);
        symtab_insert(id.name, attrs);
    }
}

// build the symbol table and check the constants in cds
void scope_check_constDecls(const_decls_t cds)
{
    const_decl_t *cdp = cds.start;
    while (cdp != NULL) {
        scope_check_constDecl(*cdp);
        cdp = cdp->next;
    }
}

void scope_check_constDecl(const_decl_t cd)
{
    id_kind type = 0;
    scope_check_constIdents(cd.const_def_list, type);
}

void scope_check_constIdents(const_def_list_t ids, id_kind t) {
    const_def_t *idp = ids.start;

    while (idp != NULL) {
        scope_check_declare_constIdent(idp->ident, t);
        idp = idp->next;
    }
}

void scope_check_declare_constIdent(ident_t id, id_kind t) {
    if (symtab_declared_in_current_scope(id.name)) {
        bail_with_prog_error(*(id.file_loc), "constant \"%s\" is already declared as a constant", id.name);    }
    else {
        int ofst_cnt = symtab_scope_loc_count();
        id_attrs *attrs = create_id_attrs(*(id.file_loc), t, ofst_cnt);
        symtab_insert(id.name, attrs);
    }
}

// build the symbol table and check the procedures in pds
void scope_check_procDecls(proc_decls_t pds)
{
    id_kind type = 2;
    proc_decl_t *pdp = pds.proc_decls;
    while (pdp != NULL) {
        scope_check_declare_procIdent(*pdp, type);
        pdp = pdp->next;
    }
}

void scope_check_declare_procIdent(proc_decl_t id, id_kind t)
{
    if (symtab_declared_in_current_scope(id.name)) {
        bail_with_prog_error(*(id.file_loc), "procedure \"%s\" is already declared as a procedure", id.name);
    }
    else {
        int ofst_cnt = symtab_scope_loc_count();
        id_attrs *attrs = create_id_attrs(*(id.file_loc), t, ofst_cnt);
        symtab_insert(id.name, attrs);
    }
}

// check the statement to make sure that
// all idenfifiers used have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
stmts_t scope_check_stmt(stmts_t stmt)
{
    if(stmt.stmts_kind == empty_stmts_e) {
        return stmt;
    }
    struct stmt_s *stmts = stmt.stmt_list.start;
    while (stmts != NULL) {
        
    switch (stmts->stmt_kind) {
    case assign_stmt:
        stmts->data.assign_stmt = scope_check_assignStmt(stmts->data.assign_stmt);
        break;
    case call_stmt:
        stmts->data.call_stmt = scope_check_callStmt(stmts->data.call_stmt);
        break;
    case if_stmt:
        stmts->data.if_stmt = scope_check_ifStmt(stmts->data.if_stmt);
        break;
    case read_stmt:
        stmts->data.read_stmt = scope_check_readStmt(stmts->data.read_stmt);
        break;
    case print_stmt:
        stmts->data.print_stmt = scope_check_printStmt(stmts->data.print_stmt);
        break;
    case while_stmt:
        stmts->data.while_stmt = scope_check_whileStmt(stmts->data.while_stmt);
        break;
    case block_stmt:
        stmts->data.block_stmt = scope_check_blockStmt(stmts->data.block_stmt);
        break;
    default:
        bail_with_error("Call to scope_check_stmt with an AST that is not a statement!");
        break;
    }
        stmts = stmts->next;
    }
    
    return stmt;
}

// check the statement for
// undeclared identifiers
// Return the modified AST with id_use pointers
assign_stmt_t scope_check_assignStmt(assign_stmt_t stmt)
{
    const char *name = stmt.name;
    id_use *idu = scope_check_ident_declared(*(stmt.file_loc), name);
    assert(idu != NULL); 
    *stmt.expr = scope_check_expr(*(stmt.expr));
    return stmt;
}

call_stmt_t scope_check_callStmt(call_stmt_t stmt)
{
    id_use *idu = scope_check_ident_declared(*(stmt.file_loc), stmt.name);
//resolves eustis warning
    free(idu);
    return stmt;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
if_stmt_t scope_check_ifStmt(if_stmt_t stmt)
{
    switch (stmt.condition.cond_kind) {
        case ck_db:
            stmt.condition.data.db_cond.dividend = scope_check_expr(stmt.condition.data.db_cond.dividend);
            stmt.condition.data.db_cond.divisor = scope_check_expr(stmt.condition.data.db_cond.divisor);
            break;
        case ck_rel:
            stmt.condition.data.rel_op_cond.expr1 = scope_check_expr(stmt.condition.data.rel_op_cond.expr1);
            stmt.condition.data.rel_op_cond.expr2 = scope_check_expr(stmt.condition.data.rel_op_cond.expr2);
            break;
        default:
            break;
    }
    
    *(stmt.then_stmts) = scope_check_stmt(*(stmt.then_stmts));
    if(stmt.else_stmts != NULL) {
        *(stmt.else_stmts) = scope_check_stmt(*(stmt.else_stmts));

    }
    return stmt;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
read_stmt_t scope_check_readStmt(read_stmt_t stmt)
{
    id_use *idu = scope_check_ident_declared(*(stmt.file_loc), stmt.name);
    free(idu);
    return stmt;
}

// check the statement to make sure that
// all idenfifiers referenced in it have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
print_stmt_t scope_check_printStmt(print_stmt_t stmt)
{
    stmt.expr = scope_check_expr(stmt.expr);
    return stmt;
}

while_stmt_t scope_check_whileStmt(while_stmt_t stmt)
{
    switch (stmt.condition.cond_kind) {
        case ck_db:
            stmt.condition.data.db_cond.dividend = scope_check_expr(stmt.condition.data.db_cond.dividend);
            stmt.condition.data.db_cond.divisor = scope_check_expr(stmt.condition.data.db_cond.divisor);
            break;
        case ck_rel:
            stmt.condition.data.rel_op_cond.expr1 = scope_check_expr(stmt.condition.data.rel_op_cond.expr1);
            stmt.condition.data.rel_op_cond.expr2 = scope_check_expr(stmt.condition.data.rel_op_cond.expr2);
            break;
        default:
            break;
    }

    *(stmt.body) = scope_check_stmt(*(stmt.body));

    return stmt;
}

block_stmt_t scope_check_blockStmt(block_stmt_t stmt)
{
    *(stmt.block) = scope_check_program(*(stmt.block));
    return stmt;
}

// check that name has been declared,
// if so, then return an id_use for it
// otherwise, produce an error 
id_use *scope_check_ident_declared(file_location floc, const char *name)
{
    id_use *ret = symtab_lookup(name);
    if (ret == NULL) {
	    bail_with_prog_error(floc, "identifier \"%s\" is not declared!", name);
    }
    assert(ret->attrs != NULL);
    return ret;
}

// check the expresion to make sure that
// all idenfifiers used have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
expr_t scope_check_expr(expr_t exp)
{
    switch (exp.expr_kind) {
        case expr_bin:
            exp.data.binary = scope_check_binary_op_expr(exp.data.binary);
            break;
        case expr_ident:
            exp.data.ident = scope_check_ident_expr(exp.data.ident);
            break;
        case expr_number:
            // no operation 
            break;
        case expr_negated:
            *(exp.data.negated.expr) = scope_check_expr(*(exp.data.negated.expr));
            break;
        default:
            bail_with_error("Unexpected expr_kind_e (%d) in scope_check_expr", exp.expr_kind);
            break;
    }
    return exp;
}

// check that all identifiers used in exp
// have been declared
// (if not, then produce an error)
// Return the modified AST with id_use pointers
binary_op_expr_t scope_check_binary_op_expr(binary_op_expr_t exp)
{
    *(exp.expr1) = scope_check_expr(*(exp.expr1));
    *(exp.expr2) = scope_check_expr(*(exp.expr2));
    return exp;
}

// check the identifier (id) to make sure that
// all it has been declared (if not, then produce an error)
// Return the modified AST with id_use pointers
ident_t scope_check_ident_expr(ident_t id)
{
    id_use *idu = scope_check_ident_declared(*(id.file_loc), id.name);
    free(idu);
    return id;
}
