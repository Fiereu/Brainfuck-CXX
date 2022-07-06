#pragma once
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <iostream>
#include <vector>

#define POINTER_INC '>'
#define POINTER_DEC '<'
#define VALUE_INC	'+'
#define VALUE_DEC	'-'
#define LOOP_START	'['
#define LOOP_END	']'
#define GET_CHAR	','
#define PUT_CHAR	'.'
#define END_OF_CODE	'\x00'

namespace Brainfuck
{
	class Memory 
	{
	private:
		size_t _pointer;
		size_t _size;
		char* _array;
		
	public:
		Memory(size_t size) 
		{
			_pointer = 0;
			_size = size;

			do {
				_array = (char*)malloc(_size);
			} while (!_array);
			
			memset(_array, 0x00, _size);
		}

		size_t pointerIncrease() 
		{
			if(_pointer < _size - 1)
				return ++_pointer;

			return _pointer;
		}

		size_t pointerDecrease() 
		{
			if (_pointer > 0)
				return --_pointer;

			return _pointer;
		}

		size_t valueIncrease() 
		{
			return ++_array[_pointer];
		}

		size_t valueDecrease() 
		{
			return --_array[_pointer];
		}

		size_t setValue(char value)
		{
			return _array[_pointer] = value;
		}

		char getValue()
		{
			return _array[_pointer];
		}

		char* dump()
		{
			return _array;
		}
	};

	char* run(Memory memory, const char* code) 
	{
		size_t instructionPointer = 0;
		std::vector<size_t> loops;

		while (code[instructionPointer] != END_OF_CODE) {
			switch (code[instructionPointer])
			{
			case POINTER_INC:
				memory.pointerIncrease();
				break;
			case POINTER_DEC:
				memory.pointerDecrease();
				break;
			case VALUE_INC:
				memory.valueIncrease();
				break;
			case VALUE_DEC:
				memory.valueDecrease();
				break;
			case LOOP_START:
				loops.push_back(instructionPointer);
				break;
			case LOOP_END:
				if (memory.getValue() == 0)
					break;
				
				if (loops.size() == 0)
					return memory.dump();

				instructionPointer = loops.back();
				loops.pop_back();
				continue;
			case GET_CHAR:
				memory.setValue(getchar());
				break;
			case PUT_CHAR:
				putchar(memory.getValue());
				break;
			default:
				break;
			}

			instructionPointer++;
		}

		return memory.dump();
	}

	typedef struct _Value
	{
		char value;
		char nextTens;
		bool isPositiv;
		char missingValue;

	} Value;

	void incValue(std::vector<char>* code, char value)
	{
		for (char i = 0; i < value; i++)
		{
			(*code).push_back(VALUE_INC);
		}
	}

	void decValue(std::vector<char>* code, char value)
	{
		for (char i = 0; i < value; i++)
		{
			(*code).push_back(VALUE_DEC);
		}
	}

	char* generate(char* output, bool shouldPrint = false)
	{
		std::vector<char> code;
		size_t outputLenght = strlen(output);
		if (shouldPrint)
		{
			Value* values = new Value[outputLenght];

			for (size_t i = 0; i < outputLenght; i++)
			{
				if (output[i] % 10 == 0) 
				{
					values[i].isPositiv = true;
					values[i].missingValue = 0;
					values[i].nextTens = output[i];
					values[i].value = output[i];
				}
				else if (output[i] % 10 > 5) 
				{
					values[i].isPositiv = false;
					values[i].missingValue = 10 - output[i] % 10;
					values[i].nextTens = (output[i] + values[i].missingValue);
					values[i].value = output[i];
				}
				else 
				{
					values[i].isPositiv = true;
					values[i].missingValue = output[i] % 10;
					values[i].nextTens = output[i] - values[i].missingValue;
					values[i].value = output[i];
				}
			}

			incValue(&code, 10);
			code.push_back(LOOP_START);
			
			for (size_t i = 0; i < outputLenght; i++)
			{
				code.push_back(POINTER_INC);
				incValue(&code, values[i].nextTens / 10);
			}

			for (size_t i = 0; i < outputLenght; i++)
			{
				code.push_back(POINTER_DEC);
			}
			code.push_back(VALUE_DEC);
			code.push_back(LOOP_END);

			for (size_t i = 0; i < outputLenght; i++)
			{
				code.push_back(POINTER_INC);
				if (values[i].isPositiv)
				{
					incValue(&code, values[i].missingValue);
				}
				else
				{
					decValue(&code, values[i].missingValue);
				}
				code.push_back(PUT_CHAR);
			}
		}
		else
		{
			for (size_t i = 0; i < outputLenght; i++)
			{
				incValue(&code, output[i]);
			}
		}

		size_t size = code.size();
		char* out = new char[size + 1];
		for (size_t i = 0; i < size; i++)
		{
			out[i] = code.at(i);
		}
		out[size] = END_OF_CODE;

		return out;
	}
}