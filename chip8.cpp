#include "chip8.h"
#include <random>
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>
void chip8::initialize(){
pc = 0x200;
opcode = 0;
I = 0;
sp = 0;
drawFlag = false;
// reset the stack and registers 
	for(int i = 0; i < 16; ++i){
		stack[i] = 0;	
		V[i] = 0;		
	}
// reset graphics display
	for(int i = 0; i < sizeof(gfx); ++i){
		gfx[i] = 0;		
	}
//clear memory
	for(int i = 0; i < sizeof(memory); ++i){
		memory[i] = 0;
	}

// load in font
	for(int i = 0; i < 80; ++i){
		memory[i] = chip8_fontset[i];
	}
// reset timers
	delay_timer = 60;
	sound_timer = 60;
}
void chip8::loadGame(const std::string& filename){
	std::ifstream file(filename, std::ios::binary | std::ios::ate);
	//read buffer
	if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
		return;
		}

	//get buffer size and buffer pointer to array char
	std::streamsize size = file.tellg();
	char* buffer = new char[size];

	if(size > 3584){
	std::cerr << "file is too large";
		file.close();
		return;
	} 
	file.seekg(0,std::ios::beg);


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
	std::cout << "PC: " << std::hex << pc << "  Opcode: " << opcode << std::endl;
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
				pc += 2;
				break;

			}else{

			for(int i = 0; i < sizeof(gfx); ++i){
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
			if((V[(opcode & 0x0F00) >> 8]) == (opcode & 0x00FF)){
				pc = pc + 4;
			}else{
				pc = pc + 2;
			}
		break;
		case 0x4000:
				if((V[(opcode & 0x0F00) >> 8]) != (opcode & 0x00FF)){
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
			pc += 2;
		break;
		case 0x7000: 
			V[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF); 
			pc += 2;
		break;

		case 0x8000:
		switch(opcode & 0x000F){
			case 0x0000:
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];	
			pc += 2;
			break;
			case 0x0001:
			V[(opcode & 0x0F00) >> 8] = (V[(opcode & 0x0F00) >> 8])|(V[(opcode & 0x00F0) >> 4]);
			pc += 2;
			break;
			case 0x0002: 
			V[(opcode & 0x0F00) >> 8]= V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
				
			case 0x0003: 
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
				
			case 0x0004:{
			uint16_t sum = V[(opcode & 0x00F0) >> 4] + V[(opcode & 0x0F00) >> 8]; 
			V[0xF] = (sum > 255) ? 1 : 0;
			V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
			} 
			case 0x0005: 

			V[0xF] = (V[(opcode & 0x0F00) >> 8] >= V[(opcode & 0x00F0) >> 4]) ? 1 : 0; 
			V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
				
			case 0x0006:{
			uint16_t lsb = V[(opcode & 0x0F00) >> 8] & 0x1;


			V[0xF] = lsb;
			V[(opcode & 0x0F00) >> 8] =  V[(opcode & 0x0F00) >> 8] >> 1; 

			pc += 2;
			break;
				} 
			
			
			case 0x0007: 
			V[0xF] = (V[(opcode & 0x00F0) >> 4]>= V[(opcode & 0x0F00) >> 8]) ? 1 : 0;
			V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;
				
			case 0x000E:{
			uint16_t msb = (V[(opcode & 0x0F00) >> 8] & 0x80) >> 7;
			V[0xF] = msb;
			V[(opcode & 0x0F00) >> 8] =  (V[(opcode & 0x0F00) >> 8]) << 1;
			pc += 2;
			break;
				}
			default: 
					
				printf("bad logic switch");	
				std::cout << "Couldn't identify opcode" << "\n";
				pc +=2;
				break;
					
	}
		break;
		case 0x9000: {
		if(V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4]){
			pc = pc + 4;
		}else{
			pc = pc + 2;
		}
			break;
				}
	
		case 0xB000: {
			pc = V[0] + (opcode & 0x0FFF);
			pc += 2;
			break;
			}
		case 0xC000:{
			int random;
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> distr(0, 255);
			random = distr(gen);
			
			
			V[(opcode & 0x0F00) >> 8]  = (opcode & 0x00FF) & random;
			pc += 2;

			break;
		}
		case 0xD000: {
  			   unsigned short x = V[(opcode & 0x0F00) >> 8];
			   unsigned short y = V[(opcode & 0x00F0) >> 4];
			   unsigned short height = opcode & 0x000F;
			   unsigned short pixel;

			   V[0xF] = 0;
			   for (int yline = 0; yline < height; yline++)
			  {
			     pixel = memory[I + yline];
			     for(int xline = 0; xline < 8; xline++)
			     {
			       if((pixel & (0x80 >> xline)) != 0)
			       {
			         if(gfx[(x + xline + ((y + yline) * 64))] == 1)
			           V[0xF] = 1;
			         gfx[x + xline + ((y + yline) * 64)] ^= 1;
			       }
			     }
			   }

			 drawFlag = true;
			 pc += 2;
			 break;
			 }
		case 0xE000:
			switch(opcode & 0x00F0){
				case 0x0090:{
					if(key[V[(opcode & 0x0F00) >> 8]] != 0)
						pc += 4;
					else
						pc+= 2;
					break;
				}
				case 0x00A0:{
					if(key[V[(opcode & 0x0F00) >> 8]] == 0)
						pc += 4;
					else
						pc+= 2;
					break;
				}
				default: {
				printf("bad E switch");	
				std::cout << "Couldn't identify opcode" << "\n";
					break;
				}
			} 
		case 0xF000:
			switch(opcode & 0x00FF){
				case 0x0007:{
					V[(opcode & 0x0F00) >> 8] = delay_timer;
					pc += 2;
					break;
				}
				case 0x000A: {
 					   uint8_t x = (opcode & 0x0F00) >> 8;
    						bool keyPressed = false;

  					  for (int i = 0; i < 16; ++i) {
       							 if (key[i]) {
           							 V[x] = i;
            							keyPressed = true;
          								  break;
      								  }
   								 }

    						if (!keyPressed)
       						 return; // Skip pc += 2, wait for key
   						 pc += 2;
    						break;
				}
				case 0x0015:{
					delay_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				}
				case 0x0018:{
					sound_timer = V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				}
				case 0x001E: {
					I += V[(opcode & 0x0F00) >> 8];
					pc += 2;
					break;
				}
				case 0x0029: {
					uint8_t Vx = (opcode & 0x0F00) >> 8;
					uint8_t digit = V[Vx];
					I = 0x50 + (5 * digit);
					pc += 2;
				}
				case 0x0033: {
					uint8_t value = V[(opcode & 0x0F00) >> 8];

					memory[I] = value/100;
					memory[I + 1] = (value/10) % 10;
					memory[I + 2] = value % 10;
					pc += 2;
					break;
				}
				case 0x0055:{
					for(int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i){
						memory[I + i] = V[i];
					}
					I += (ushort) (((opcode & 0x0F00) >> 8) + 1);
					pc += 2;
					break;
				}
				case 0x0065:{
					for(int i = 0; i <= ((opcode & 0x0F00) >> 8); ++i){
						V[i] = memory[I + i];
					}
					I += (ushort)(((opcode % 0x0F00) >> 8) + 1);
					pc += 2;
					break;	
				}
				default: {
				printf("opcode bad LAST");			
				std::cout << "Couldn't identify opcode" << "\n";
					break;
				}

			}
	}
	//update timer
	if(delay_timer > 0){
	--delay_timer;

	}
	if(sound_timer > 0){
		if(sound_timer == 1){
			printf("BEEP /n");
		}
	 --sound_timer;
	}
}
void chip8::clearScreen(){
	for(int i = 0; i < sizeof(gfx); ++i){
		gfx[i] = 0;		
	}
}

