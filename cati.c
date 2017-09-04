
#include "cati.h"

void	getTerminalSize(t_winSize *terminalSize)
{
	struct winsize screen_winSize;
	
	ioctl(0, TIOCGWINSZ, &screen_winSize);
	printf("Screen width: %i \n", screen_winSize.ws_col);
	terminalSize->width = screen_winSize.ws_col;
	terminalSize->height = screen_winSize.ws_row;
}

unsigned char	*readImage(char **argv, int *bytesPerPixel, t_winSize *size)
{
	t_winSize			sizeOrig;
	unsigned char	*imageOrig;
	t_winSize			terminalSize;
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
		
		// resize image
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





int main(int argc, char **argv)
{

	t_winSize			size;
	int				bytesPerPixel;
	unsigned char	*image;
	

	image = readImage(argv, &bytesPerPixel, &size);

	argc = 2;
	

	t_rgb	*rgb = (t_rgb*)malloc(sizeof(t_rgb) * size.width * size.height);

	

	
	int totalLen = size.width * size.height * bytesPerPixel;
	int pos = 0;
	int a = 0;
	while(pos < totalLen)
	{
		rgb[a].r = image[pos];
		rgb[a].g = image[pos + 1];
		rgb[a].b = image[pos + 2];
		rgb[a].a = image[pos + 3];
		pos += bytesPerPixel;
		a++;
	}
	
	int x;
	int y;
	

	y = 0;
	a = 0;
	while(y < size.height)
	{
		x = 0;
		while(x < size.width)
		{

			if (bytesPerPixel == 4)
				printf("\x1b[0;48;2;%d;%d;%d;%dm ", rgb[a].r, rgb[a].g, rgb[a].b, rgb[a].a);
			else if (bytesPerPixel == 3)
				printf("\x1b[0;48;2;%d;%d;%dm ", rgb[a].r, rgb[a].g, rgb[a].b);
			else if (bytesPerPixel == 2)
				printf("\x1b[0;48;2;%d;%dm ", rgb[a].r, rgb[a].g);
			else if (bytesPerPixel == 1)
				printf("\x1b[0;48;2;%dm ", rgb[a].r);
			a++;
			x++;
		}
		printf("\033[0m\n");
		y++;
	}
	stbi_image_free(image);
	return 0;
}
