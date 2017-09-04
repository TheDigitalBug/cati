

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

 #define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

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




#include <sys/ioctl.h> // get terminal window width

void	getTerminalWidth(int *terminalHeight, int *terminalWidth)
{
	struct winsize screen_size;
	
	ioctl(0, TIOCGWINSZ, &screen_size);
	printf("Screen width: %i \n", screen_size.ws_col);
	*terminalWidth = screen_size.ws_col;
	*terminalHeight = screen_size.ws_row;
}

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

int	hexTo256(t_rgb rgb)
{
	int	r = (rgb.r < 75 ? 0 : (rgb.r-35) /40) * 6 * 6;
	int	g = (rgb.g < 75 ? 0 : (rgb.g - 35 ) / 40) * 6;
	int	b = (rgb.b < 75 ? 0 : (rgb.b - 35) / 40) + 16;
	return(r+g+b);
}


int main(int argc, const char **argv)
{
	int				widthOrig;
	int				heightOrig;
	int				bytesPerPixelOrig;
	unsigned char	*imageOrig;
	
	int				width;
	int				height;
	int				bytesPerPixel;
	unsigned char	*image;
	
	
	int				terminalWidth;
	int				terminalHeight;
	double			ratioOrig;
	
	argc = 2;
	

	
	imageOrig = stbi_load(argv[1], &widthOrig, &heightOrig, &bytesPerPixelOrig, 4);
	if (!imageOrig)
	{
		puts("allocation failure or image is corrupt/invalid");
		exit(0);
	}

	printf("bytesPerPixel=%d\n", bytesPerPixelOrig);
	printf("width=%d\n", widthOrig);
	printf("height=%d\n", heightOrig);
	getTerminalWidth(&terminalHeight, &terminalWidth);
	

	
	
	ratioOrig  = (double) widthOrig / heightOrig;
	printf("ratio=%f\n", ratioOrig);
	
	
	if (widthOrig > (terminalWidth / 2))
	{
		width = terminalWidth / 2;
		height = (double)width / ratioOrig;
	}
	else
	{
		width = widthOrig;
		height = heightOrig;
	}
	
	
	
	image = (unsigned char	*)malloc(sizeof(unsigned char) * width * height * bytesPerPixelOrig);
	
	stbir_resize_uint8_srgb( imageOrig , widthOrig , heightOrig , 0,
							image, width, height, 0,
							bytesPerPixelOrig , 0  , 0);
	
	bytesPerPixel = bytesPerPixelOrig;
	
	
	

	printf("New width=%d\n", width);
	printf("New height=%d\n", height);
	
	t_rgb	*rgb = (t_rgb*)malloc(sizeof(t_rgb) * width*height);


	SDL_Init(SDL_INIT_EVERYTHING);
	
	t_sdl sdl;
	sdl.window = SDL_CreateWindow("test window", 0, 0, width, height, SDL_WINDOW_OPENGL);
	sdl.renderer = SDL_CreateRenderer(sdl.window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl.renderer);
	

	
	int totalLen = width * height * bytesPerPixel;
	int pos = 0;
	int a = 0;
	while(pos < totalLen)
	{
		rgb[a].r = image[pos];
		rgb[a].g = image[pos + 1];
		rgb[a].b = image[pos + 2];
		rgb[a].a = image[pos + 3];
		pos += bytesPerPixelOrig;
		a++;
	}
	
	
	int x;
	int y;
	y = 0;
	a = 0;
	while(y < height)
	{
		x = 0;
		while(x < width)
		{
			SDL_SetRenderDrawColor(sdl.renderer, rgb[a].r, rgb[a].g, rgb[a].b, rgb[a].a);
			SDL_RenderDrawPoint(sdl.renderer, x, y);
			
			if (bytesPerPixelOrig == 4)
				printf("\x1b[0;48;2;%d;%d;%d;%dm  ", rgb[a].r, rgb[a].g, rgb[a].b, rgb[a].a);
			else if (bytesPerPixelOrig == 3)
				printf("\x1b[0;48;2;%d;%d;%dm  ", rgb[a].r, rgb[a].g, rgb[a].b);
				printf("\033[0m");
			
			
			
			a++;
			x++;
		}
		printf("\n");
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
	stbi_image_free(image);
	return 0;
}
