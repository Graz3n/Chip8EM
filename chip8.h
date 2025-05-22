
#include <string>
#include <fstream>
class chip8{
	unsigned short opcode; // the current opcode
	unsigned char memory[4096]; //all memory (bytes)
	unsigned char V[16]; // all registers
	unsigned short I; // index register
	unsigned short pc; // program counter
	unsigned char gfx[64 * 32]; //graphics
	unsigned char delay_timer; // the 60hz timer
	unsigned char sound_timer; // 60hz timer for making sounds
	unsigned short stack[16]; // stack for calls and jumps
	unsigned short sp; // for stack pointer
	unsigned char key[16];	

public:
	void initialize();
public:
	void emulateCycle();
public:
	void loadGame(const std::string& filename);

};
