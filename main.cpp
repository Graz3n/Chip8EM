#include <chrono>
#include <SDL2/SDL.h>
#include "chip8.h" // Your cpu core implementation
chip8 myChip8;
void drawGraphics(uint8_t* gfx);
void setupGraphics();
void setKeys();
const int VIDEO_WIDTH = 64;
const int VIDEO_HEIGHT = 32;
const int PIXEL_SIZE = 10;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

int main(int argc, char **argv) {
	const int cycleDelay = 1000/60;
	setupGraphics();
	myChip8.initialize();
	myChip8.loadGame("CHIP8GAMES/pumpkindressup.ch8");
	auto lastCycleTime = std::chrono::high_resolution_clock::now(); 
	for(;;){
		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();
		if(dt > cycleDelay){
		lastCycleTime = currentTime;

		myChip8.emulateCycle();
			uint8_t* graphicsP = myChip8.gfx;
		if(myChip8.drawFlag){
			drawGraphics(graphicsP);
			myChip8.drawFlag = false;
			}
			
		setKeys();
		}

		}
	return 0;
 }

void setupGraphics(){ 
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Chip-8 emulator",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, VIDEO_WIDTH * PIXEL_SIZE, VIDEO_HEIGHT * PIXEL_SIZE, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}
void drawGraphics(uint8_t* gfx){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white pixels

    for (int y = 0; y < VIDEO_HEIGHT; ++y) {
        for (int x = 0; x < VIDEO_WIDTH; ++x) {
            if (gfx[y * VIDEO_WIDTH + x]) {
                SDL_Rect pixel = {
                    x * PIXEL_SIZE,
                    y * PIXEL_SIZE,
                    PIXEL_SIZE,
                    PIXEL_SIZE
                };
                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);

}
void setKeys(){
	SDL_Event e;
	while(SDL_PollEvent(&e)){
		switch(e.type){
			case SDL_KEYDOWN:{
				switch(e.key.keysym.sym){
			case SDLK_1:
			myChip8.key[0] = 1; 
			break;
			case SDLK_2:
			myChip8.key[1] = 1; 
			break;
			case SDLK_3:
			myChip8.key[2] = 1; 
			break;
			case SDLK_4:
			myChip8.key[3] = 1; 
			break;
			case SDLK_q:
			myChip8.key[4] = 1; 
			break;
			case SDLK_w:
			myChip8.key[5] = 1; 
			break;
			case SDLK_e:
			myChip8.key[6] = 1; 
			break;
			case SDLK_r:
			myChip8.key[7] = 1; 
			break;
			case SDLK_a:
			myChip8.key[8] = 1; 
			break;
			case SDLK_s:
			myChip8.key[9] = 1; 
			break;
			case SDLK_d:
			myChip8.key[10] = 1; 
			break;
			case SDLK_f:
			myChip8.key[11] = 1; 
			break;
			case SDLK_z:
			myChip8.key[12] = 1; 
			break;
			case SDLK_x:
			myChip8.key[13] = 1; 
			break;
			case SDLK_c:
			myChip8.key[14] = 1; 
			break;
			case SDLK_v:
			myChip8.key[15] = 1; 
			break;
				}

			}
			break;
			case SDL_KEYUP:{
				switch(e.key.keysym.sym){
			case SDLK_1:
			myChip8.key[0] = 0; 
			break;
			case SDLK_2:
			myChip8.key[1] = 0; 
			break;
			case SDLK_3:
			myChip8.key[2] = 0; 
			break;
			case SDLK_4:
			myChip8.key[3] = 0; 
			break;
			case SDLK_q:
			myChip8.key[4] = 0; 
			break;
			case SDLK_w:
			myChip8.key[5] = 0; 
			break;
			case SDLK_e:
			myChip8.key[6] = 0; 
			break;
			case SDLK_r:
			myChip8.key[7] = 0; 
			break;
			case SDLK_a:
			myChip8.key[8] = 0; 
			break;
			case SDLK_s:
			myChip8.key[9] = 0; 
			break;
			case SDLK_d:
			myChip8.key[10] = 0; 
			break;
			case SDLK_f:
			myChip8.key[11] = 0; 
			break;
			case SDLK_z:
			myChip8.key[12] = 0; 
			break;
			case SDLK_x:
			myChip8.key[13] = 0; 
			break;
			case SDLK_c:
			myChip8.key[14] = 0; 
			break;
			case SDLK_v:
			myChip8.key[15] = 0; 
			break;


				}
			}

		}
		
	}

}


