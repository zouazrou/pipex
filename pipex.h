#ifndef PIPEX_H
# define PIPEX_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#define NUM 6 // remove

typedef struct pipex_s
{
    char	***cmd; // ex :{ {"/usr/bin/ls", "-l", NULL}, {"/usr/bin/grep", ".c", NULL}, {"/usr/bin/wc", "-l", NULL}, (char *)NULL }
    char	**env;
    int		*pid;
    int		count;
    int		num_cmd;
    int		prev_fd;

}   pipex_t;

// Libft
void	*ft_calloc(size_t count, size_t size);
char	**ft_split(char const *s, char c);

pipex_t	*init_struct(pipex_t *data, int num, char **env);
// pipex_t *init_struct(int num, char **env);
void	exec_first(pipex_t *data);
void	exec_middle(pipex_t *data);
void	exec_last(pipex_t *data);
void	clean_all(pipex_t *data, int stat);

#endif
