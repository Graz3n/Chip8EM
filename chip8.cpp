#include <iostream>
#include "chip8.h"
#include <string>
#include <fstream>
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



}







