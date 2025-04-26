#ifndef PIPEX_H
# define PIPEX_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

typedef struct pipex_s
{
    char    **path;
    char    ***cmd;
    char    **env;
    int     count;
    int     prev_fd;

}   pipex_t;

void    init_struct(pipex_t *data, int num, char **env);
void    exec_first(pipex_t *data);
void    exec_middle(pipex_t *data);
void    exec_last(pipex_t *data);

#endif