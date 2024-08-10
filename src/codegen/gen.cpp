#include "frontend/lex.h"
#include "frontend/tree.h"
#include "codegen/gen.h"
#include <iostream>

static int label(void) {
  static int id = 1;
  return (id++);
}

static int gen_if(an_ast_node* ifnode)
{
	int Lfalse, Lend;
	// Generate two labels: one for the
	// false compound statement, and one
	// for the end of the overall IF statement.
	// When there is no ELSE clause, Lfalse _is_
	// the ending label!
	Lfalse = label();
	if (ifnode->right)
		Lend = label();

	// Generate the condition code followed
	// by a zero jump to the false label.
	// We cheat by sending the Lfalse label as a register.
	gen_AST(ifnode->left, Lfalse, ifnode->op);
	gen_free_regs();

	// Generate the true compound statement
	gen_AST(ifnode->mid, NOREG, ifnode->op);
	gen_free_regs();

	// If there is an optional ELSE clause,
	// generate the jump to skip to the end
	if (ifnode->right)
	cg_jump(Lend);

	// Now the false label
	cg_label(Lfalse);

	// Optional ELSE clause: generate the
	// false compound statement and the
	// end label
	if (ifnode->right) {
		gen_AST(ifnode->right, NOREG, ifnode->op);
		gen_free_regs();
		cg_label(Lend);
	}

	return (NOREG);
}
static int gen_while(an_ast_node* while_node)
{
	int Lstart, Lend;

	Lstart = label();
	Lend = label();
	
	cg_label(Lstart);

	gen_AST(while_node->left , Lend , while_node->op);
	gen_free_regs();

	gen_AST(while_node->right , Lstart , while_node->op);
	gen_free_regs();

	cg_jump(Lstart);
	cg_label(Lend);

	return (NOREG);	
}

static int gen_do_while(an_ast_node* do_while_node)
{
	int Lstart , Lend;

	Lstart = label();
	Lend = label();

	cg_label(Lstart);

	gen_AST(do_while_node->right , NOREG, do_while_node->op);
	gen_free_regs();

	gen_AST(do_while_node->left , Lend, do_while_node->op);
	gen_free_regs();

	cg_jump(Lstart);
	cg_label(Lend);

	return(NOREG);

}
int gen_AST(an_ast_node *n , int reg , an_ast_node_kind parent_op)
{
	switch (n->op)
	{
		case an_ast_node_kind::node_if:
			return (gen_if(n));
		case an_ast_node_kind::node_while:
			return (gen_while(n));
		case an_ast_node_kind::node_do_while:
			return (gen_do_while(n));
		case an_ast_node_kind::node_glue:
			{
				gen_AST(n->left, NOREG , n->op);
				gen_free_regs();
				gen_AST(n->right, NOREG, n->op);
				gen_free_regs();
				return (NOREG);
			}
		case an_ast_node_kind::node_func:
			{
				cg_func_prologue(get_sym_name( std::get<a_symtable_index>( n->value)));
				gen_AST(n->left,NOREG,n->op);
				cg_func_epilogue(std::get<a_symtable_index>( n->value));
				return NOREG;
			}

	}

	int leftreg, rightreg;
	// Get the left and right sub-tree values
	if (n->left != nullptr)
		leftreg = gen_AST(n->left , NOREG , n->op);
	if (n->right != nullptr)
		rightreg = gen_AST(n->right , leftreg , n->op);

	switch (n->op)
	{
		case an_ast_node_kind::node_add:
			return (cgadd(leftreg,rightreg));
		case an_ast_node_kind::node_sub:
			return (cgsub(leftreg,rightreg));
		case an_ast_node_kind::node_mul:
			return (cgmul(leftreg,rightreg));
		case an_ast_node_kind::node_div:
			return (cgdiv(leftreg,rightreg));
		case an_ast_node_kind::node_int_literal:
		  	{
				return (cgload( std::get<int>( std::get<a_number>( n->value))));
			}
		case an_ast_node_kind::node_ident:
			return (cgload_sym( get_sym_name( std::get<a_symtable_index>( n->value))));
		case an_ast_node_kind::node_lvalue:
			return (cgstr_sym(reg,get_sym_name( std::get<a_symtable_index>( n->value))));
		case an_ast_node_kind::node_assign:
			return rightreg;
		case an_ast_node_kind::node_print:
			{
				gen_printint(leftreg);
				gen_free_regs();
				return (NOREG);
			}
		case an_ast_node_kind::node_eq_eq:
		case an_ast_node_kind::node_neq:
		case an_ast_node_kind::node_lt:
		case an_ast_node_kind::node_gt:
		case an_ast_node_kind::node_le:
		case an_ast_node_kind::node_ge:
		 	{
				if ( is_jump_node(parent_op))
				{
					return cg_compare_and_jump(n->op,leftreg,rightreg,reg);
				}
				else{
					return (cg_compare_and_set(n->op , leftreg , rightreg));
				}
			}
		case an_ast_node_kind::node_widen:
			return cg_widen(leftreg , n->left->type_kind, n->type_kind);
		
		case an_ast_node_kind::node_return:
			cg_return(leftreg , get_sym_name( std::get<a_symtable_index>( n->value)));
			return (NOREG);
			
		case an_ast_node_kind::node_func_call:
			return cg_call(leftreg , get_sym_name( std::get<a_symtable_index>( n->value)));
	}
}