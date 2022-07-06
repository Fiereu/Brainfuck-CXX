#include <iostream>

#include "Brainfuck.h"

int main()
{
	Brainfuck::Memory memory(100);
	char* code = Brainfuck::generate((char*)"Hello", true);
	printf("Code: %s\n", code);
	Brainfuck::run(memory, code);
}