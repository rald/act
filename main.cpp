#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <string>

#include <cstdio>
#include <ctime>

#include "sweetie.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool quit = false;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Event event;

struct Calamansi {
	double x,y;
	double xd,yd;
	double xs,ys;
};

Calamansi calamansi[10];



SDL_Surface *LoadImage(std::string path) {
	SDL_Surface *surface = SDL_LoadBMP(path.c_str());

 	if(surface == NULL) {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        exit(-1);
    }

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, sweetie[0].r, sweetie[0].g, sweetie[0].b));	

	return surface;
}

void DrawImage(SDL_Renderer *renderer,SDL_Surface *surface,int x,int y) {
	SDL_Rect renderQuad = {x, y, surface->w, surface->h};
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
	SDL_DestroyTexture(texture);
}

void Speed(int i) {
	calamansi[i].xs=(rand()%10+1)*10;
	calamansi[i].ys=(rand()%10+1)*10;
}

int main(int argc,char *argv[]) {

	srand(time(NULL));

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n",SDL_GetError());
		exit(-1);
    }
	
	window = SDL_CreateWindow( 
		"Abe's Calamansi Tycoon", 
		SDL_WINDOWPOS_UNDEFINED, 
		SDL_WINDOWPOS_UNDEFINED, 
		SCREEN_WIDTH, 
		SCREEN_HEIGHT, 
		SDL_WINDOW_SHOWN 
	);
	
	if(window == NULL) {
    	printf("Window could not be created! SDL_Error: %s\n",SDL_GetError());
		exit(-1);
    }
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if(renderer == NULL) {
		printf("Renderer could not be created! SDL Error: %s\n",SDL_GetError());
		exit(-1);
	}

	SDL_Surface *gameTitleImage = LoadImage("images/GameTitle.bmp");
	SDL_Surface *abeImage = LoadImage("images/abe.bmp");
	SDL_Surface *calamansiImage = LoadImage("images/calamansi.bmp");
	SDL_Surface *slicedCalamansiImage = LoadImage("images/SlicedCalamansi.bmp");

	SDL_Surface *leftArrowImage = LoadImage("images/LeftArrow.bmp");
	SDL_Surface *rightArrowImage = LoadImage("images/RightArrow.bmp");

	SDL_Surface *numbersImage = LoadImage("images/numbers.bmp");


	
	for(int i=0;i<10;i++) {
		calamansi[i].x=rand()%SCREEN_WIDTH;
		calamansi[i].y=rand()%SCREEN_HEIGHT;
		calamansi[i].xs=(rand()%10+1)*10;
		calamansi[i].ys=(rand()%10+1)*10;
		calamansi[i].xd=rand()%2?1:-1;
		calamansi[i].yd=rand()%2?1:-1;	
	}


	// High-precision tracking variables
	Uint64 lastTime = SDL_GetPerformanceCounter();
	Uint64 currentTime = 0;
	double deltaTime = 0.0;

	while(!quit) {
		
		// 1. Calculate Delta Time
        currentTime = SDL_GetPerformanceCounter();
        
        // deltaTime in seconds = (Count difference) / Counts per second
        deltaTime = static_cast<double>(currentTime - lastTime) / static_cast<double>(SDL_GetPerformanceFrequency());
        lastTime = currentTime;

        // 2. Clamp Delta Time (Crucial for handling pauses, window drags, or debugging breakpoints)
        if (deltaTime > 0.1) {
            deltaTime = 0.1; 
        }

		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
			case SDL_QUIT: quit = true; break;
			case SDL_KEYDOWN: 
				switch(event.key.keysym.sym) {
				case SDLK_ESCAPE: quit = true; break;
				}
				break;
			default: break;
			}
		}
		
		SDL_SetRenderDrawColor(renderer,sweetie[6].r,sweetie[6].g,sweetie[6].b,0xFF);
		SDL_RenderClear(renderer);
		
		for(int i=0;i<10;i++) {
			DrawImage(renderer,slicedCalamansiImage,calamansi[i].x,calamansi[i].y);

			calamansi[i].x+=calamansi[i].xd * calamansi[i].xs * deltaTime;
			calamansi[i].y+=calamansi[i].yd * calamansi[i].ys * deltaTime;

			if(calamansi[i].x < 0) { calamansi[i].x=0; calamansi[i].xd=1; Speed(i); }
			if(calamansi[i].y < 0) { calamansi[i].y=0; calamansi[i].yd=1; Speed(i); }

			if(calamansi[i].x > SCREEN_WIDTH-slicedCalamansiImage->w) { calamansi[i].x=SCREEN_WIDTH-slicedCalamansiImage->w; calamansi[i].xd=-1; Speed(i); }
			if(calamansi[i].y > SCREEN_HEIGHT-slicedCalamansiImage->h) { calamansi[i].y=SCREEN_HEIGHT-slicedCalamansiImage->h; calamansi[i].yd=-1;  Speed(i);}
		}

		DrawImage(renderer,gameTitleImage,(SCREEN_WIDTH-gameTitleImage->w)/2,10);
		DrawImage(renderer,calamansiImage,(SCREEN_WIDTH-calamansiImage->w)/2,100);

		DrawImage(renderer,abeImage,SCREEN_WIDTH-abeImage->w,SCREEN_HEIGHT-abeImage->h);

		DrawImage(renderer,leftArrowImage,0,0);
		DrawImage(renderer,rightArrowImage,64,0);

		DrawImage(renderer,numbersImage,32,64);
		
		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}


