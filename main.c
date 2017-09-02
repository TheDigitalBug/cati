

#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"




// Basic usage (see HDR discussion below for HDR usage):
//    int x,y,n;
//    unsigned char *data = stbi_load(filename, &x, &y, &n, 0);
//    // ... process data if not NULL ...
//    // ... x = width, y = height, n = # 8-bit components per pixel ...
//    // ... replace '0' with '1'..'4' to force that many components per pixel
//    // ... but 'n' will always be the number that it would have been if you said 0
//    stbi_image_free(data)
//
// Standard parameters:
//    int *x                 -- outputs image width in pixels
//    int *y                 -- outputs image height in pixels
//    int *channels_in_file  -- outputs # of image components in image file
//    int desired_channels   -- if non-zero, # of image components requested in result


# include <SDL2/SDL.h>

# define H HEIG / 30

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Renderer		*renderer;
	SDL_Event		e;
}					t_sdl;


typedef struct		s_rgb
{
	int	r;
	int g;
	int b;
	int a;
}					t_rgb;






int main(int argc, const char **argv)
{
	int width;
	int height;
	int bytesPerPixel;
	unsigned char *image;

 
	
	
	
	image = stbi_load("s.png", &width, &height, &bytesPerPixel, 0);
	if (!image)
	{
		puts("allocation failure or image is corrupt/invalid");
		exit(0);
	}
	t_rgb	*rgb = (t_rgb*)malloc(sizeof(t_rgb) * width*height);
	printf("bytesPerPixel=%d\n", bytesPerPixel);
	printf("width=%d\n", width);
	printf("height=%d\n", height);
	
	
	SDL_Init(SDL_INIT_EVERYTHING);
	
	t_sdl sdl;
	sdl.window = SDL_CreateWindow("Pacman", 0, 0, width, height, SDL_WINDOW_OPENGL);
	sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl.renderer);
	
	int x;
	int y;
	
	y = 0;
	int a = 0;
	while(y < height)
	{
		x = 0;
		
		while(x < width)
		{
			rgb[a].r = image[y*width + x];
			rgb[a].g = image[y*width + x + 1];
			rgb[a].b = image[y*width + x + 2];
			rgb[a].a = image[y*width + x + 3];
//			printf("%d\n",y*width + x);
			a++;
			x+=4;
		}
		y++;
//		printf("\n");
	}
	
	y = 0;
	a = 0;
	while(y < height)
	{
		x = 0;
		while(x < width)
		{
			SDL_SetRenderDrawColor(sdl.renderer, rgb[a].r, rgb[a].g, rgb[a].b, rgb[a].a);
			SDL_RenderDrawPoint(sdl.renderer, x, y);
			a++;
			x++;
			printf("a=`%d\n",a);
		}
		y++;
	}
	
	
	
	
	
	SDL_RenderPresent(sdl.renderer);
	
	
	int stop = 0;
	while (!stop)
	{
		while (SDL_PollEvent(&(sdl.e)))
		{
			if (sdl.e.type == SDL_QUIT || sdl.e.key.keysym.sym == SDLK_ESCAPE)
				stop = 1;
		}
	}

	
	
	
	
//	int i = 0;
//	
//	while (i < width)
//	{
//		printf("r = %d\n", image[i++]);
//		printf("g = %d\n", image[i++]);
//		printf("b = %d\n", image[i++]);
//		printf("a = %d\n", image[i++]);
//	}
//
//	
//	
//	
//	puts(argv[1]);
//	printf("%s\n\n", image);
//	
//	
	stbi_image_free(image);
	return 0;
}
