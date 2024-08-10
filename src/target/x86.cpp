#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>

#include<string_view>
#include "frontend/lex.h"
#include "frontend/tree.h"
#include "codegen/gen.h"


extern std::ofstream asm_file;
// List of available registers
static int freereg[4];
static std::string reglist[4]= { "%r8", "%r9", "%r10", "%r11" };
static std::string breglist[4]= {"%r8b", "%r9b", "%r10b", "%r11b"};
static std::string dreglist[4]= {"%r8d", "%r9d", "%r10d", "%r11d"};

// Set all registers as available
static void freeall_registers(void)
{
	freereg[0]= freereg[1]= freereg[2]= freereg[3]= 1;
}

// Allocate a free register. Return the number of
// the register. Die if no available registers.
static int alloc_register(void)
{
	for (int i=0; i<4; i++) {
		if (freereg[i]) {
			freereg[i]= 0;
			return(i);
		}
	}
	std::cerr<< "Out of registers!\n";
	exit(1);
}

// Return a register to the list of available registers.
// Check to see if it's not already there.
static void free_register(int reg)
{
	if (freereg[reg] != 0) {
		std::cerr<<"internal error:  trying to free register " << reglist[reg]<< "\n";
		exit(1);
	}
	freereg[reg]= 1;
}

// Print out the assembly prologue
void cgprologue()
{
	freeall_registers();
	asm_file<<
		"\t.text\n"
		".LC0:\n"
		"\t.string\t\"%d\\n\"\n"
		"printint:\n"
		"\tpushq\t%rbp\n"
		"\tmovq\t%rsp, %rbp\n"
		"\tsubq\t$16, %rsp\n"
		"\tmovl\t%edi, -4(%rbp)\n"
		"\tmovl\t-4(%rbp), %eax\n"
		"\tmovl\t%eax, %esi\n"
		"\tleaq	.LC0(%rip), %rdi\n"
		"\tmovl	$0, %eax\n"
		"\tcall	printf@PLT\n"
		"\tnop\n"
		"\tleave\n"
		"\tret\n"
		"\n";
}
void cg_func_prologue(std::string name)
{
	asm_file<<
		"\t.text\n"
		"\t.globl\t"
		<<name<<"\n"
		"\t.type\t"
		<<name <<", @function\n"
		<<name<<":\n \tpushq\t%rbp\n"
		"\tmovq\t%rsp, %rbp\n";
}

// Print out the func epilogue
void cg_func_epilogue(a_symtable_index idx)
{
	cg_label(idx);
	asm_file<<
		"\tpopq	%rbp\n"
		"\tret\n";
}

// Load an integer literal value into a register.
// Return the number of the register
int cgload(int value) {

	// Get a new register
	int r= alloc_register();

	// Print out the code to initialise it
	asm_file<<"\tmovq\t$"<<value<<","<<reglist[r]<< "\n";
	return(r);
}

// Add two registers together and return
// the number of the register with the result
int cgadd(int r1, int r2) {
	asm_file<<"\taddq\t"<<reglist[r1]<<","<< reglist[r2]<<"\n";
	free_register(r1);
	return(r2);
}

// Subtract the second register from the first and
// return the number of the register with the result
int cgsub(int r1, int r2) {
	asm_file<<"\tsubq\t"<<reglist[r2]<<","<< reglist[r1]<<"\n";
	free_register(r2);
	return(r1);
}

// Multiply two registers together and return
// the number of the register with the result
int cgmul(int r1, int r2) {
	asm_file<<"\timulq\t"<<reglist[r1]<<","<< reglist[r2]<<"\n";
	free_register(r1);
	return(r2);
}

// Divide the first register by the second and
// return the number of the register with the result
int cgdiv(int r1, int r2) {
	asm_file<<"\tmovq\t"<<reglist[r1]<<", %rax\n";
	asm_file<<"\tcqo\n";
	asm_file<<"\tidivq\t"<<reglist[r2]<<"\n";
	asm_file<<"\tmovq\t"<<"%rax,"<<reglist[r1]<<"\n";
	free_register(r2);
	return(r1);
}

int cgload_sym(std::string sym_name)
{
	int reg = alloc_register();
	switch(get_sym_type(sym_name))
	{
		case a_type_kind::Int:
			asm_file<<"\tmovzbl\t"<<sym_name<<"(%rip),"<< dreglist[reg]<<"\n";
		break;

		case a_type_kind::Char:
			asm_file<<"\tmovzbq\t"<<sym_name<<"(%rip),"<<breglist[reg]<<"\n";
		break;

		case a_type_kind::Long:
			asm_file<<"\tmovzbq\t"<<sym_name<<"(%rip),"<<reglist[reg]<<"\n";
		break;
	
		default:
			std::cerr<<"bad type in cgload_sym\n";
			exit(1);
	}
	return reg;
}

int cgstr_sym(int reg, std::string sym_name)
{
	switch(get_sym_type(sym_name))
	{
		case a_type_kind::Int:
			asm_file<<"\tmovl\t"<<dreglist[reg]<<","<<sym_name<<"(%rip)\n";
		break;

		case a_type_kind::Char:
			asm_file<<"\tmovb\t"<<breglist[reg]<<","<<sym_name<<"(%rip)\n";
		break;

		case a_type_kind::Long:
			asm_file<<"\tmovq\t"<<reglist[reg]<<","<<sym_name<<"(%rip)\n";
		break;

		default:
			std::cerr<<"bad type in cgstr_sym\n";
			exit(1);
	}
	return reg;
}

void cgglobal_sym(std::string sym_name)
{
	 	int type_sz = type_size[(int)get_sym_type(sym_name) - (int)a_type_kind::none];
		asm_file<<"\t.comm\t"<<sym_name<<","<<type_sz<<","<<type_sz<<"\n";
}

// Call printint() with the given register
void cgprintint(int r)
{
	asm_file<<"\tmovq\t"<<reglist[r]<<", %rdi\n";
	asm_file<<"\tcall\tprintint\n";
	free_register(r);
}

static std::string setlist[] = {"sete" , "setne", "setl" , "setg" , "setle" , "setge" };
int cg_compare_and_set(an_ast_node_kind op , int r1 , int r2)
{
	assert(is_comparison_node(op));

	asm_file<<"\tcmpq\t"<<reglist[r2]<<","<<reglist[r1]<<"\n";
	asm_file<<"\t"<<setlist[(int)op - (int)an_ast_node_kind::node_eq_eq]<<"\t"<<breglist[r2]<<"\n";
	asm_file<<"\tmovzbq\t"<<breglist[r2]<<","<<reglist[r2]<<"\n";
	free_register(r1);
	return r2;
}

static std::string jumplist[] = { "jne", "je", "jge", "jle", "jg", "jl" };

// Compare two registers and jump if false.
int cg_compare_and_jump(an_ast_node_kind op, int r1, int r2, int label) {
	
	assert(is_comparison_node(op));
	asm_file<<"\tcmpq\t"<<reglist[r2]<<","<<reglist[r1]<<"\n";
	asm_file<<"\t"<<jumplist[(int)op - (int)an_ast_node_kind::node_eq_eq]<<"\tL"<<label<<"\n";
	freeall_registers();
	return (NOREG);
}

// Generate a label
void cg_label(int l) {
	asm_file<<"L"<<l<<":\n";
}

// Generate a jump to a label
void cg_jump(int l) {
	asm_file<<"\tjmp\tL"<<l<<"\n";
}

int cg_widen(int r , a_type_kind oldtype , a_type_kind newtype)
{
	return r;
}

void cg_return(int reg , std::string name)
{

switch(get_sym_type(name))
	{
		case a_type_kind::Int:
			asm_file<<"\tmovl\t"<<dreglist[reg]<<","<<"%eax\n";
		break;

		case a_type_kind::Char:
			asm_file<<"\tmovb\t"<<breglist[reg]<<","<<"%eax\n";
		break;

		case a_type_kind::Long:
			asm_file<<"\tmovq\t"<<reglist[reg]<<","<<"%rax\n";
		break;

		default:
			std::cerr<<"bad type in cg_return\n";
			exit(1);
	}
	cg_jump(find_symbol(name));
}

int cg_call(int reg , std::string name)
{
	int return_reg = alloc_register();
	asm_file<<"\tmovq\t"<<reglist[reg]<<",%rdi\n";
	asm_file<<"\tcall\t"<<name<<"\n";
	asm_file<<"\tmovq\t"<<"%rax,"<<reglist[return_reg]<<"\n";
	free_register(reg);
	return return_reg;
}
void gen_prologue()        { cgprologue(); }
void gen_free_regs()        { freeall_registers(); }
void gen_printint(int reg) { cgprintint(reg); }
void gen_globalsym(std::string name){cgglobal_sym(name);}