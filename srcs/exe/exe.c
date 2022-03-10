#include "minishell.h"
#include "exe.h"

void	__cmd_list_clear(t_cmd *start)
{
	t_cmd	*next_to_free;

	while (start)
	{
		next_to_free = start->next;
		free_split(start->arg);
		free(start);
		start = next_to_free;
	}
}

static void __cmd_add_back(t_cmd **alst, t_cmd *new)
{
	t_cmd	*nextlst;

	if (!(*alst))
		*alst = new;
	else
	{
		nextlst = (*alst);
		while (nextlst->next)
			nextlst = nextlst->next;
		nextlst->next = new;
	}
}

int	__get_nb_param_cmd(t_lexing *start)
{
    int size;

    size = 0;
    while (start && start->type != PIPE)
    {
        size++;
        start = start->next;
    }
	return (size);
}

t_cmd	*create_new_cmd(int nb_param, int index, t_msh *msh)
{
    t_cmd *new;

    new = malloc(sizeof(t_cmd));
    if(!new)
        return(NULL);
    new->next = NULL;
    new->redirection[0] = -1;
    new->redirection[1] = -1;
    new->index = index;
    new->msh = msh;
    new->arg = malloc(sizeof(char *) * (nb_param + 1));
    if(!new->arg)
        return (free(new), NULL);
    new->arg[nb_param] = NULL;
	return (new);
}

int add_next_cmd(t_cmd **start, t_lexing **lexing, t_msh *msh, int index)
{
    int			i;
    t_cmd		*new;
	t_lexing	*save;

    i = 0;
	new = create_new_cmd(__get_nb_param_cmd(*lexing), index, msh);
	if (!new)
		return (0);
    while(*lexing && (*lexing)->type != PIPE)
    {
        new->arg[i] = (*lexing)->token;
		save = *lexing;
        *lexing = (*lexing)->next;
		free (save);
        i++;
    }
    if (*lexing)
	{
		save = *lexing;
        *lexing = (*lexing)->next;
		free(save->token);
		free(save);
	}
    __cmd_add_back(start, new);
    return (1);
}

t_cmd *create_cmd_list(t_lexing *lexing, t_msh *msh)
{
    t_cmd *start;
    int i;

    i = 0;
    start = NULL;
    while (lexing)
    {
        if (!add_next_cmd(&start, &lexing, msh, i))
            return (__cmd_list_clear(start), NULL);
        i++;
    }
    return (start);
}

/*
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
*/

void print_cmd_lst(t_cmd *cmd)
{
    while (cmd)
    {
        print_cmd(cmd);
        cmd = cmd->next;
        printf("--\n");
    }
}
void print_cmd(t_cmd *cmd)
{
    int i;

    i = 0;
    printf("stdin = %d stdout = %d, index = %d\n", cmd->redirection[0], cmd->redirection[1], cmd->index);
    while(cmd->arg[i])
    {
        printf("arg %d >%s<\n",i , cmd->arg[i]);
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

static int __find_max_cmd(t_cmd *cmd)
{
    int i;

    i = 0;
    while (cmd)
    {
        cmd = cmd->next;
        i++;
    }
    return (i);
}

static char **__create_envp(char ***envp)
{
    int size;
    int i;
    char **ret;

    i = 0;
    size = 0;
    while (envp[size])
        size++;
    ret = malloc(sizeof(char *) * size);
    if (!ret)
        return (NULL);
    while (i < size)
    {
        ret[i] = envp[i][0];
        i++;
    }
    return (ret);
}

int	__init_seq(t_sequ *seq, char ***envp, t_cmd *cmd)
{
    seq->pipe[in] = -1;
    seq->pipe[out] = -1;
    seq->index = 0;
    seq->max_cmd = __find_max_cmd(cmd);
    if (__find_path(envp))
    {
        seq->path = __split(__find_path(envp), ':');
        if (!seq->path)
            return (0);
    }
    seq->envp = __create_envp(envp);
    if (!seq->envp)
        return (0);
    return (1);
}

void D_print_seq(t_sequ *seq)
{
    int i;

    i = 0;
    printf("max cmd = %d\n", seq->max_cmd);
    while (seq->path[i])
    {
        printf("path = %s\n", seq->path[i]);
        i++;
    }
}

/*
static char	*__create_path(char *path, char *cmd)
{
	char	*tmp;
	char	*tmp2;

	tmp = NULL;
	tmp = __strjoin(__strdup(path), "/");
	if (!tmp)
		return (NULL);
	tmp2 = __strjoin(tmp, cmd);
	if (!tmp2)
		return (NULL);
	return (tmp2);
}
*/

char	*__create_path_and_cmd(char *path, char *cmd)
{
	char	*ret;
	int		size;
	int		i;

	i = 0;
	if (!path || !cmd)
		return (NULL);
	size = __strlen(path) + __strlen("/") + __strlen(cmd);
	ret = (char *)malloc((size + 1) * sizeof(char));
	if (!ret)
		return (__putstr_fd("Malloc Error", 2), NULL);
	ret[size] = '\0';
	while (path[i])
	{
		ret[i] = path[i];
		i++;
	}
	ret[i] = '/';
	i = 0;
	while (cmd[i])
	{
		ret[i + __strlen(path) + __strlen("/")] = cmd[i];
		i++;
	}
	return (ret);
}

int	__try_paths(char **path_cmd, char **path, char *cmd_name)
{
    int	i;

    i = 0;
	while (path[i])
	{
		*path_cmd = __create_path_and_cmd(path[i], cmd_name);
		if (!*path_cmd)
			return(__putstr_fd("MALLOC ERROR\n", 2), 0);
		if (access(*path_cmd, F_OK) == 0)
			return (1);
		i++;
		free(*path_cmd);
	}
	return (0);
}

static char *__get_path(char **path, char *cmd_name)
{
    int		i;
    char	*path_cmd;

    i = 0;
	if (__strchr(cmd_name, '/') != NULL)
	{
		if (__strncmp(cmd_name, "./", 2) == 0 && access(cmd_name + 2, F_OK) == 0)
				return (cmd_name + 2);
		if (access(cmd_name, F_OK) == 0)
			return (cmd_name);
		if (cmd_name[__strlen(cmd_name) - 1] == '/')
            return (NULL);
	}
	if (path == NULL)
		return (NULL);
	else
    {
		if (__try_paths(&path_cmd, path, cmd_name) == 1)
			return (path_cmd);
    }
    return(__putstr_fd("Command not found\n", 2), NULL);
}

void execute_child(t_sequ *seq, t_cmd *cmd)
{
    char *path_cmd;

    path_cmd = __get_path(seq->path, cmd->arg[0]);
    execve(path_cmd, cmd->arg, seq->envp);
    exit (0);
}

int __connect_pipe(int pipe[2],t_cmd *cmd, int index, int prev_out )
{
    (void)cmd;
    if (dup2(pipe[out], STDOUT_FILENO) < 0)
        return (0);
    //close(pipe[out]);
    if (index != 0)
    {
        dup2(prev_out, STDIN_FILENO);
        close(prev_out);
    }
    //close(pipe[in]);
    /*
    if (cmd->redirection[0] > -1)
    {
        dup2(cmd->redirection[0], STDIN_FILENO);
    }
     if (cmd->redirection[0] > -1)
    {
        close(pipe[out]);
        dup2(cmd->redirection[0], STDOUT_FILENO);
    }
    */
    return (1);
}

int __launcher_fork(t_sequ *seq, t_cmd *cmd)
{
    pid_t pid;
    int status;
    int ret;
    int prev_out;
    
    prev_out = 0;
    while (seq->index < seq->max_cmd)
    {
        if (pipe(seq->pipe) < 0)
            return (__putstr_fd("Error pipe\n", 2), 0);
        pid = fork();
        if (pid < 0)
            return (__putstr_fd("Error fork\n", 2), 0);
        if (pid == 0)
        {
            __connect_pipe(seq->pipe, cmd, seq->index, prev_out);
            execute_child(seq, cmd);
        }
        else
        {
            seq->index++;
            cmd = cmd->next;
            close(seq->pipe[out]);
            if(seq->index != 0)
                close(prev_out);
            prev_out = seq->pipe[in];
        }
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status) > 0)
		ret = (WEXITSTATUS(status));
	if (WIFSIGNALED(status) > 0)
		ret = (WTERMSIG(status));
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (ret);
}

int __is_builtin(char **arg)
{
	if (__strncmp(arg[0], "echo", 4) == 0)
        return (1);
	if (__strncmp(arg[0], "cd", 2) == 0)
         return (1);
	if (__strncmp(arg[0], "pwd", 3) == 0)
         return (1);
	if (__strncmp(arg[0], "env", 3) == 0)
        return (1);
	if (__strncmp(arg[0], "export", 6) == 0)
         return (1);
	if (__strncmp(arg[0], "unset", 5) == 0)
        return (1);
    return (0);
}

void	__exec_builtin(char **arg, t_msh *msh)
{

	if (__strncmp(arg[0], "echo", 4) == 0)
        msh->rv =__echo(arg, 1);
	if (__strncmp(arg[0], "cd", 2) == 0)
        msh->rv = __cd(arg[1], msh);
	if (__strncmp(arg[0], "pwd", 3) == 0)
        msh->rv = __pwd(1);
	if (__strncmp(arg[0], "env", 3) == 0)
        msh->rv = __env(msh);
	if (__strncmp(arg[0], "export", 6) == 0)
        msh->rv = __export(arg + 1, msh);
	if (__strncmp(arg[0], "unset", 5) == 0)
        msh->rv = __unset(arg + 1, msh);
}

int execute_seq(t_cmd *cmd, t_msh *msh)
{
    t_sequ seq;

    if (!__init_seq(&seq, msh->envp, cmd))
        return (__putstr_fd("Malloc error\n", 2), 0);
    if (seq.max_cmd == 1 && __is_builtin(cmd->arg))
	{
		__exec_builtin(cmd->arg, msh);
		free_split(seq.path);
		free(seq.envp);
		__cmd_list_clear(cmd);
        return (0);
	}
    msh->rv = __launcher_fork(&seq, cmd);
	free_split(seq.path);
	free(seq.envp);
	__cmd_list_clear(cmd);
    return (0);
}
