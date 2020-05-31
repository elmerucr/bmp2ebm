#include <cstdio>

#include <SDL2/SDL.h>


int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);

	atexit( SDL_Quit );

	SDL_Surface *bmp_to_load;

	bmp_to_load = SDL_LoadBMP("test.bmp");

	if(bmp_to_load == nullptr)
	{
		printf("[bmp2ebm] error: %s\n", SDL_GetError() );
		exit(1);
	}

	printf("%u x %u bmp\n", bmp_to_load->w, bmp_to_load->h);

	//SDL_PixelFormat *this_format = 

	printf("pixelformat: %s", bmp_to_load->format);




	SDL_FreeSurface(bmp_to_load);

	SDL_Quit();

	return 0;
}