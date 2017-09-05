
NAME = cati

CC = gcc

CFLAGS =  -F. -Wall -Werror -Wextra

OBJ = cati.o

HEADER = cati.h

all : $(NAME)

$(NAME) : $(OBJ)
	@$(CC)  $^ $(CFLAGS)  -o $@

clean:
	@rm -rf $(OBJ)
	
fclean: clean
	@rm -rf $(NAME)
	
re: fclean all
