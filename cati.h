
#ifndef cati_h
#define cati_h

// read image library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// resize image library
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

// get terminal window width
#include <sys/ioctl.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct		s_rgb
{
	int				r;
	int				g;
	int				b;
	int				a;
}					t_rgb;


typedef struct		s_option
{
	int				trueColor;
	int				resizeToTerminal;
}					t_option;

typedef struct		s_winSize
{
	int				height;
	int				width;
}					t_winSize;

void				getTerminalSize(t_winSize *terminalSize);
unsigned char		*readImage(char **argv, int *bytesPerPixel, t_winSize *size, t_option option);
int					hexTo256(t_rgb rgb);
t_option			optionParser(char **argv);
void				drawImage(int bytesPerPixel, unsigned char *image, t_winSize size, t_option option);
#endif
