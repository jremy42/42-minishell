#include "minishell.h"
#include "exe.h"

t_cmd *miniparsing(t_lexing *lexing)
{
    t_cmd *cmd;
    t_lexing *tmp;
    int i;
    int j;

    tmp = lexing;
    cmd = malloc(sizeof(t_cmd));
    cmd->redirection[0] = 0;
    cmd->redirection[1] = 1;
    i = 0;
    j = 0;

    while(tmp)
    {
        if (tmp->type == WORD)
            i++;
        tmp = tmp->next;
    }
    i++;
    cmd->arg = malloc(sizeof(char *) * i);
    __bzero(cmd->arg, i);
    tmp = lexing;
    i = 0;
    while(tmp != NULL)
    {
        while (tmp != NULL && tmp->type == WORD)
        {
            if (tmp->type == PIPE)
                break;
            if(j == 0)
                cmd->arg[i] = __strdup("");
            cmd->arg[i] = __strjoin(cmd->arg[i], tmp->token);
            cmd->arg[i] = __strjoin(cmd->arg[i],__strdup(" "));

            tmp = tmp->next;
            j++;
        }
        j = 0;
        if (tmp && tmp->next)
            tmp = tmp->next;
        i++;
    }
    cmd->arg[i] = NULL;
    return(cmd);
}

void print_cmd(t_cmd *cmd)
{
    int i;

    i = 0;
    printf("stdin = %d stdout = %d\n", cmd->redirection[0], cmd->redirection[1]);
    while(cmd->arg[i])
    {
        printf("cmd %d >%s<\n",i , cmd->arg[i]);
        i++;
    }
}

static char	*__find_path(char ***envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (__strncmp(envp[i][0], "PATH", 4) == 0)
			return (envp[i][0] + 5);
		i++;
	}
	return (NULL);
}

static int __find_max_cmd(char **cmd)
{
    int i;

    i = 0;
    while (cmd[i])
        i++;
    return (i);
}

static char**__create_envp(char ***envp)
{
    int size;
    int i;
    char **ret;

    i = 0;
    size = 0;
    while(envp[size])
        size++;
    ret = malloc(sizeof(char *) * size);
    while (i < size)
    {
        ret[i] = __strdup(envp[i][0]);
        i++;
    }
    return (ret);
}

void	__init_seq(t_sequ *seq, int redirect[2], char ***envp, t_cmd *cmd)
{
    seq->std_in = redirect[0];
    seq->std_out = redirect[1];
    seq->pipe[in] = 0;
    seq->pipe[out] = 0;
    seq->index = 0;
    seq->max_cmd = __find_max_cmd(cmd->arg);
    seq->path = __find_path(envp);
    seq->envp = __create_envp(envp);
    seq->av = cmd->arg;
}

void D_print_seq(t_sequ *seq)
{
    printf("std_in = %d\n", seq->std_in);
    printf("std_out = %d\n", seq->std_out);
    printf("max cmd = %d\n", seq->max_cmd);
    printf("path = %s\n", seq->path);
}

int __launcher_fork(t_sequ *seq)
{
    int i;
    pid_t pid;

	i = 0;
    i = 10000;
    while (i < seq->max_cmd)
    {
        if (i == 0)
        pid = fork();
    }
	return (0);
}

int execute_seq(t_cmd *cmd, char ***envp)
{
    t_sequ seq;

    __init_seq(&seq, cmd->redirection, envp, cmd);
    __launcher_fork(&seq);
    print_cmd(cmd);
    D_print_seq(&seq);
    return (0);

}
