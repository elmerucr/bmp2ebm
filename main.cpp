/*	picture2asm (main.cpp)
 *
 *	Convert a picture into an m68k assembler format appropriate for E64-II
 *
 *	Copyright © 2020 elmerucr. All rights reserved.
 */

#include <cstdio>
#include <cstdint>
#define  STB_IMAGE_IMPLEMENTATION
#define  STBI_FAILURE_USERMSG
#include "stb_image.h"

#define	VERSION_MAJOR	0
#define VERSION_MINOR	1
#define	BUILD			20200606

/*	Values 0x00 to 0xff must be transformed to values 0x0 to 0xf. This can't be done
*	by simply shifting 4 bits to the right. So, 0x00 must be in the
*	'middle' of 0x0, 0x11 must be in the middle of 0x1, etc... The right
*	integer transformation can be achieved by adding 8 and dividing by 17.
*/
inline uint8_t transform(uint8_t value)
{
	return (value+8)/17;
}

uint16_t sizes[8][2] =
{
	{ 8, 0 },
	{ 16, 1 },
	{ 32, 2 },
	{ 64, 3 },
	{ 128, 4 },
	{ 256, 5 },
	{ 512, 6 },
	{ 1024, 7 }
};

int main(int argc, char **argv)
{
	if( argc == 1 )
	{
		printf("[picture2asm] error: called without arguments, exiting...\n");
		exit(1);
	}
    printf("[picture2asm] trying to open '%s'\n", argv[1]);

    int x,y,n;

    unsigned char *data = stbi_load(argv[1], &x, &y, &n, 0);
	if(data == nullptr)
	{
		printf("[picture2asm] error: %s\n", stbi_failure_reason() );
		printf("[picture2asm] exiting...\n");
		exit(1);
	}

	printf("[picture2asm] size %u x %u, totally %u pixels, %u bytes per pixel\n", x, y, x*y, n);

	uint8_t width_tile_log2 = 0xff;		// initial "wrong" value
	uint8_t height_tile_log2 = 0xff;	// initial "wrong" value

	// assume wrong sizes at first

	bool wrong_width = true;
	bool wrong_height = true;
	for(int i=0; i<8; i++)
	{
		if( x == sizes[i][0] )
		{
			wrong_width = false;
			width_tile_log2 = sizes[i][1];
		}
		if( y == sizes[i][0] )
		{
			wrong_height = false;
			height_tile_log2 = sizes[i][1];
		}
	}

	if( wrong_width | wrong_height )
	{
		printf("[picture2asm] error: image width or height not a power of 2, not a multiple of 8 or larger than 1024\n");
		printf("[picture2asm] exiting...\n");
		exit(1);
	}

	const char *size_in_binary[] =
	{
		"%00000000",
		"%00000001",
		"%00000010",
		"%00000011",
		"%00000100",
		"%00000101",
		"%00000110",
		"%00000111"
	};

	printf("\talign	5\t\t; blit data must be 32 bytes (2^5) aligned\n");
	printf("<something>\n");
	printf("\tDC.B\t%%00000101\t; flags 0 - multicolor and bitmap mode\n");
	printf("\tDC.B\t%%00000000\t; flags 1 - empty\n");
	printf("\tDC.B\t%s\t; width 2^%u = %u tile(s) = %u pixels\n", size_in_binary[width_tile_log2], width_tile_log2, 1 << width_tile_log2, 1 << (width_tile_log2 + 3));
	printf("\tDC.B\t%s\t; height 2^%u = %u tile(s) = %u pixels\n", size_in_binary[height_tile_log2], height_tile_log2, 1 << height_tile_log2, 1 << (height_tile_log2 + 3));
	printf("\tDC.W\t$0000\t\t; x\n");
	printf("\tDC.W\t$0000\t\t; y\n");
	printf("\tDC.W\t$0000\t\t; foreground color\n");
	printf("\tDC.W\t$0000\t\t; background color\n");
	printf("\tDC.L\t<something>\t; pixel_data\n");
	printf("\tDC.L\t$0\t\t; character_data\n");
	printf("\tDC.L\t$0\t\t; character_color_data\n");
	printf("\tDC.L\t$0\t\t; background_color_data\n");
	printf("\tDC.L\t$0\t\t; user_data\n");

	uint8_t alpha;

	for(int i=0; i < n*x*y; i += n)
	{
		if(!((i/n) % 16)) printf("\n\tDC.W\t");

		switch( n )
		{
			case 1:
				break;
			case 2:
				break;
			case 3:
				alpha = 0xF;
				break;
			case 4:
				alpha = transform(data[i+3]);
				break;
		}
		printf("$%01X%01X%01X%01X%c", alpha, transform(data[i]), transform(data[i+1]), transform(data[i+2]), (((i/n)+1) % 16) ? ',' : 0);
	}

	printf("\n");

    return 0;
}
