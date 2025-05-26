#include <iostream>
#include "chip8.h"
#include <string>
#include <fstream>
#include <cstdint>
void chip8::initialize(){
pc = 0x200;
opcode = 0;
I = 0;
sp = 0;

// reset the stack and registers 
	for(int i = 0; i < 16; ++i){
		stack[i] = 0;	
		V[i] = 0;		
	}
// reset graphics display
	for(int i = 0; i < sizeof(gfx) / sizeof(gfx[0]) ; ++i){
		gfx[i] = 0;		
	}
//clear memory
	for(int i = 0; i < sizeof(memory) / sizeof(memory[0]); ++i){
		memory[i] = 0;
	}

// load in font
	for(int i = 0; i < 80; ++i){
	}
// reset timers
}
void chip8::loadGame(const std::string& filename){
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	//read buffer
	std::filebuf* pbuf = file.rdbuf();
	//get buffer size and buffer pointer to array char
	std::streamsize size = file.tellg();

	if(size > 3584){
	std::cerr << "file is too large";
		file.close();
		return;
	} 
	file.seekg(0,std::ios::beg);

	char* buffer = new char[size];

	file.read(buffer,size);	
	file.close();
	//load buffer into memory:
	
	for(int i = 0; i < size; i++){
		memory[0x200 + i] = buffer[i];
	}

	delete[] buffer;
}

void chip8::emulateCycle(){
//fetch some opcode:
	opcode = memory[pc] << 8 | memory[pc + 1]; 

	switch(opcode & 0xF000) {
		case 0xA000:
			I = opcode & 0x0FFF;
			pc = pc + 2;
			break;
		case 0x0000:
			if(opcode == 0x00EE){
				//go back
				pc = stack[sp - 1];
				sp--;
				pc = pc + 2;
				break;

			}else{

			for(int i = 0; i < sizeof(gfx) / sizeof(gfx[0]) ; ++i){
				gfx[i] = 0;		
			}
				pc = pc + 2;
				break;
			}

		case 0x1000:
			pc = opcode & 0x0FFF;
			break;
		case 0x2000:
			stack[sp] = pc;
			sp++;
			pc = opcode & 0x0FFF;		
			break;
		case 0x3000:
			if(V[(opcode & 0x0F00) >> 8] == opcode & 0x00FF){
				pc = pc + 4;
			}else{
				pc = pc + 2;
			}
		break;
		case 0x4000:
				if(V[(opcode & 0x0F00) >> 8] != opcode & 0x00FF){
				pc = pc + 4;
			}else{
				pc = pc + 2;
			}
		break;
		case 0x5000:
			if(V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4]){
				pc = pc + 4;
			}else{
				pc = pc + 2;
			}
		break;
		case 0x6000:
			V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
		break;
		case 0x7000: 
			V[(opcode & 0x0F00) >> 8] =  V[(opcode & 0x0F00) >> 8] + opcode & 0x00FF; 
		break;

		case 0x8000:

		switch(opcode & 0x000F){
			case 0x0000:
			V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4];	
			break;
			case 0x0001:
			char X = V[(opcode & 0x0F00) >> 8];
			char Y = V[(opcode & 0x00F0) >> 4];
			V[X] = X | Y;
			break;
			case 0x0002:
			char X = V[(opcode & 0x0F00) >> 8];
			char Y = V[(opcode & 0x00F0) >> 4];
			V[X] = X & Y;
			break;
			case 0x0003:
			char X = V[(opcode & 0x0F00) >> 8];
			char Y = V[(opcode & 0x00F0) >> 4];
			V[X] = X ^ Y;
			break;
			case 0x0004:
			char X = V[(opcode & 0x0F00) >> 8];
			char Y = V[(opcode & 0x00F0) >> 4];
			uint8_t sum = Y + X; 
			V[0xF] = (sum > 255) ? 1 : 0;
			break;
			case 0x0005:
			char X = V[(opcode & 0x0F00) >> 8];
			char Y = V[(opcode & 0x00F0) >> 4];


			V[0xF] = (V[X] >= V[Y]) ? 1 : 0; 
			break;
			case 0x0006:
			char X = (V[(opcode & 0x0F00) >> 8]);
			uint16_t lsb = X & 0x1;


			V[0xF] = lsb;
			V[(opcode & 0x0F00) >> 8] =  X >> 1; 
			break;
			case 0x0007:
			char X = V[(opcode & 0x0F00) >> 8];
			char Y = V[(opcode & 0x00F0) >> 4];
			V[0xF] = (V[X] >= V[Y]) ? 1 : 0;
			V[X] = Y - X;
			break;
			case 0x000E:
			char X = (V[(opcode & 0x0F00) >> 8]);
			uint16_t msb = (X & 0x80) >> 7;
			V[0xF] = msb;
			V[(opcode & 0x0F00) >> 8] =  X << 1;
			break;
			default:
			std::cout << "Couldn't identify opcode" << "\n";
			break;
		}
		case 0x9000:
		if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
			pc = pc + 4;
		}else{
			pc = pc + 2;
		}
	}

}







