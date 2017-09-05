
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


typedef struct		s_flag
{
	int				trueColor;	
}					t_flag;

typedef struct		s_winSize
{
	int				height;
	int				width;
}					t_winSize;

#endif
