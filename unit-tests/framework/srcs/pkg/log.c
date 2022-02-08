/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 10:04:17 by ngiroux           #+#    #+#             */
/*   Updated: 2022/01/09 17:45:52 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"

int __init_file(t_pipe *std, int file)
{
    std->stdout_cpy = dup(STDOUT_FILENO);

    
	if (dup2(file, STDOUT_FILENO) == -1)
	{
		__close_std(std);
		return (-1);
	}
	return (0);
}

void	__close_file(t_pipe *std, int file)
{

	dup2(std->stdout_cpy, STDOUT_FILENO);
	close(std->stdout_cpy);
    close (file);
}


void __log_name(char *fun, char *name)
{
    int file;
    t_pipe std;

    std.stdin_cpy = 0;
    std.stdout_cpy = 0;
    file = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (file == -1)
        return;
    if (__init_file(&std, file) == -1)
        return;
    fflush(stdout);
    printf("%s > [%s] : \n",fun, name);
    __close_file(&std, file);
    }

    void __log_test(char *test, char *att, char* ret)
    {
    int file;
    t_pipe std;

    std.stdin_cpy = 0;
    std.stdout_cpy = 0;
    file = open("log.txt", O_WRONLY | O_CREAT | O_APPEND, 0777);
    if (file == -1)
        return;
    if (__init_file(&std, file) == -1)
        return;
    fflush(stdout);
    printf("\n test : %s  \n    attendu : [%s]\n    retour : [%s]  \n",test, att, ret);
    __close_file(&std, file);
    }