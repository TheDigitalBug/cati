
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

unsigned char	*readImage(char **argv, int *bytesPerPixel, t_winSize *size, t_option option)
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
	
	if (((sizeOrig.width <= terminalSize.width) && (sizeOrig.height <= terminalSize.height)) || option.resizeToTerminal == 1)
	{
		size->width = sizeOrig.width;
		size->height = sizeOrig.height;
		return(imageOrig);
	}
	else
	{
		size->width = terminalSize.width;
		size->height = (double)terminalSize.height / ratioOrig;
		image = (unsigned char	*)malloc(sizeof(unsigned char) * size->width * size->height * (*bytesPerPixel));
		if (!image)
		{
			puts("allocation failure or image is corrupt/invalid");
			exit(0);
		}
		stbir_resize_uint8_srgb(imageOrig, sizeOrig.width, sizeOrig.height, 0,
								image, size->width, size->height, 0,
								*bytesPerPixel, 0, 0);
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

t_option			optionParser(char **argv)
{
	t_option	option;
	int		i;
	
	option.trueColor = 0;
	option.resizeToTerminal = 0;
	i = 2;
	while(argv[i])
	{
		if(strcmp(argv[i], "-t") == 0)
			option.trueColor = 1;
		if(strcmp(argv[i], "-o") == 0)
			option.resizeToTerminal = 1;
		i++;
	}
	return(option);
}

void				drawImage(int bytesPerPixel, unsigned char *image, t_winSize size, t_option option)
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

	i = 0;
	for (y = 0; y < size.height; y++)
	{
		for (x = 0; x < size.width; x++)
		{
			if (option.trueColor == 1)
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
	t_option			option;

	if(argc == 1)
	{
		printf("Usage: ./cati image.png [-options]\n");
		printf("     -t  - true color (works only if your console accepts true color env)\n");
		printf("     -o  - original resolution of image\n");
		exit(0);
	}
	option = optionParser(argv);
	image = readImage(argv, &bytesPerPixel, &size, option);
	drawImage(bytesPerPixel, image, size, option);
	stbi_image_free(image);
	return 0;
}
