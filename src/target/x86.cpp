#include <iostream>
#include <fstream>
#include <cstdio>
#include "frontend/lex.h"


extern std::ofstream asm_file;
// List of available registers
// and their names
static int freereg[4];
static std::string reglist[4]= { "%r8", "%r9", "%r10", "%r11" };

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
		"\n"
		"\t.globl\tmain\n"
		"\t.type\tmain, @function\n"
		"main:\n"
		"\tpushq\t%rbp\n"
		"\tmovq	%rsp, %rbp\n";
}

// Print out the assembly epilogue
void cgepilogue()
{
	asm_file<<
		"\tmovl	$0, %eax\n"
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

// Call printint() with the given register
void cgprintint(int r) {
	asm_file<<"\tmovq\t"<<reglist[r]<<", %rdi\n";
	asm_file<<"\tcall\tprintint\n";
	free_register(r);
}

void gen_prologue()        { cgprologue(); }
void gen_epilogue()        { cgepilogue(); }
void gen_free_regs()        { freeall_registers(); }
void gen_printint(int reg) { cgprintint(reg); }
