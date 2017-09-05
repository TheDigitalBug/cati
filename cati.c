
#include "cati.h"

// get terminal app size

void				getTerminalSize(t_winSize *terminalSize)
{
	struct winsize screen_winSize;

	ioctl(0, TIOCGWINSZ, &screen_winSize);
	printf("Screen width: %i \n", screen_winSize.ws_col);
	terminalSize->width = screen_winSize.ws_col;
	terminalSize->height = screen_winSize.ws_row;
}

// get image info - size, array of collor

unsigned char	*readImage(char **argv, int *bytesPerPixel, t_winSize *size)
{
	t_winSize		sizeOrig;
	unsigned char	*imageOrig;
	t_winSize		terminalSize;
	unsigned char	*image;
	double			ratioOrig;

	// read image and ger original size
	imageOrig = stbi_load(argv[1], &sizeOrig.width, &sizeOrig.height, bytesPerPixel, 4);
	if (!imageOrig)
	{
		puts("allocation failure or image is corrupt/invalid");
		exit(0);
	}
	ratioOrig  = (double) sizeOrig.width / sizeOrig.height;
	
	getTerminalSize(&terminalSize);
	
	if (sizeOrig.width > terminalSize.width)
	{
		size->width = terminalSize.width;
		size->height = (double)size->width / ratioOrig;
		
		image = (unsigned char	*)malloc(sizeof(unsigned char) * size->width * size->height * (*bytesPerPixel));
		stbir_resize_uint8_srgb(imageOrig, sizeOrig.width, sizeOrig.height, 0,
								image, size->width, size->height, 0,
								*bytesPerPixel, 0, 0);
	}
	else
	{
		size->width = sizeOrig.width;
		size->height = sizeOrig.height;
		return(imageOrig);
	}
	stbi_image_free(imageOrig);
	return(image);
}

int				hexTo256(t_rgb rgb)
{
	int	r = (rgb.r < 75 ? 0 : (rgb.r-35) /40) * 6 * 6;
	int	g = (rgb.g < 75 ? 0 : (rgb.g - 35 ) / 40) * 6;
	int	b = (rgb.b < 75 ? 0 : (rgb.b - 35) / 40) + 16;
	return(r+g+b);
}

t_flag			flagParser(char **argv)
{
	t_flag	flag;
	int		i;
	
	flag.trueColor = 0;
	i = 2;
	while(argv[i])
	{
		if(strcmp(argv[i], "-t") == 0)
			flag.trueColor = 1;
		i++;
	}
	return(flag);
}

void				drawImage(int bytesPerPixel, unsigned char *image, t_winSize size, t_flag flag)
{
	t_rgb	*rgb;
	int		totalLen;
	int		pos;
	int		i;
	int		x;
	int		y;

	rgb = (t_rgb*)malloc(sizeof(t_rgb) * size.width * size.height);
	
	totalLen = size.width * size.height * bytesPerPixel;
	pos = 0;
	i = 0;
	while(pos < totalLen)
	{
		rgb[i].r = image[pos];
		rgb[i].g = image[pos + 1];
		rgb[i].b = image[pos + 2];
		rgb[i].a = image[pos + 3];
		pos += bytesPerPixel;
		i++;
	}
	
	y = 0;
	i = 0;
	for (y = 0; y < size.height; y++)
	{
		for (x = 0; x < size.width; x++)
		{
			if (flag.trueColor == 1)
			{
				if (bytesPerPixel == 4)
					printf("\x1b[0;48;2;%d;%d;%d;%dm ", rgb[i].r, rgb[i].g, rgb[i].b, rgb[i].a);
				else if (bytesPerPixel == 3)
					printf("\x1b[0;48;2;%d;%d;%dm ", rgb[i].r, rgb[i].g, rgb[i].b);
				else if (bytesPerPixel == 2)
					printf("\x1b[0;48;2;%d;%dm ", rgb[i].r, rgb[i].g);
				else if (bytesPerPixel == 1)
					printf("\x1b[0;48;2;%dm ", rgb[i].r);
			}
			else
				printf("\x1b[48;05;%dm ", hexTo256(rgb[i]));
			
			i++;
		}
		printf("\033[0m\n");
	}
	free(rgb);
}

int				main(int argc, char **argv)
{
	t_winSize		size;
	int				bytesPerPixel;
	unsigned char	*image;
	t_flag			flag;

	if(argc == 1)
	{
		puts("Usage: ./cati image.png");
		exit(0);
	}
	flag = flagParser(argv);
	image = readImage(argv, &bytesPerPixel, &size);
	drawImage(bytesPerPixel, image, size, flag);
	stbi_image_free(image);
	return 0;
}
