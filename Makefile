CC = cc
# valgrind --track-fds=yes
# (gdb) set follow-fork-mode child  # Attach to child instead of parent
# (gdb) break some_function        # Set breakpoints
# (gdb) run                        # Start execution
CFLAGS = -g

SRC = main.c utils.c
OBJ = $(SRC:.c=.o)

NAME = pipex

all : $(NAME)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
#include "pipex.h"

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re run
