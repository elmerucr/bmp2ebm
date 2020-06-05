#include <cstdio>
#define  STB_IMAGE_IMPLEMENTATION
#define  STBI_FAILURE_USERMSG
#include "stb_image.h"

int main(int argc, char **argv)
{
    printf("called with: %s\n", argv[1]);

    int x,y,n;

    unsigned char *data = stbi_load(argv[1], &x, &y, &n, 0);

    printf("width: %u, height: %u, bytes per pixel: %u\n\n", x, y, n);

    for(int i=0; i < 4*x*y; i +=4)
    {
        printf("0x%02x%02x%02x%02x ", data[i+3], data[i], data[i+1], data[i+2]);
        printf("$%01X%01X%01X%01X\n", (data[i+3]+8)/17, (data[i]+8)/17, (data[i+1]+8)/17, (data[i+2]+8)/17);
    }

    return 0;
}
