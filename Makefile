
NAME = imageViewer

CC = gcc

CFLAGS =  -F. -Wall -Werror -Wextra

SDL_FLAGS = -framework SDL2 -framework

OBJ = main.o

#HEADER = pacman.h

all : $(NAME)

$(NAME) : $(OBJ)
	@$(CC)  $^ $(CFLAGS) $(SDL_FLAGS) -o $@

clean:
	@rm -rf $(OBJ)
	
fclean: clean
	@rm -rf $(NAME)
	
re: fclean all

sdl2:
	cp -r SDL2.framework ~/Library/Frameworks/

