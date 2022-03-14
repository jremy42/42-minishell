/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 09:51:56 by jremy             #+#    #+#             */
/*   Updated: 2022/03/14 12:38:10 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"

static void    __less_redirection(t_sequ *seq, t_cmd *cmd)
{
    DEBUG && fprintf(stderr, "less\n");
    cmd->redirection[0] = open(cmd->redirect->file_name, O_RDONLY);
    if (cmd->redirection[0] < 0)
        __exit_child(seq, cmd, errno, 1);
    if (dup2(cmd->redirection[0], 0) < 0)
        __exit_child(seq, cmd, errno, 1);
    if (close(cmd->redirection[0]) < 0)
        __exit_child(seq, cmd, errno, 1);
}

static void    __great_redirection(t_sequ *seq, t_cmd *cmd)
{
    DEBUG && fprintf(stderr,"great\n");
    cmd->redirection[1] = open(cmd->redirect->file_name, O_CREAT | O_WRONLY | O_TRUNC, 00644);
    if (cmd->redirection[1] < 0)
        __exit_child(seq, cmd, errno, 1);
    if (dup2(cmd->redirection[1], 1) < 0)
        __exit_child(seq, cmd, errno, 1);
    if (close(cmd->redirection[1]) < 0)
        __exit_child(seq, cmd, errno, 1);
}

static void    __dgreat_redirection(t_sequ *seq, t_cmd *cmd)
{
    DEBUG && fprintf(stderr,"Dgreat\n");
    cmd->redirection[1] = open(cmd->redirect->file_name, O_CREAT | O_WRONLY | O_APPEND, 00644);
    if (cmd->redirection[1] < 0)
        __exit_child(seq, cmd, errno, 1);
    if (dup2(cmd->redirection[1], 1) < 0)
        __exit_child(seq, cmd, errno, 1);
    if (close(cmd->redirection[1]) < 0)
        __exit_child(seq, cmd, errno, 1);
}

static void    __here_doc_redirection(t_sequ *seq, t_cmd *cmd)
{
    char *tmp_file;
    int     fd;

    (void)seq;
    DEBUG && fprintf(stderr, "here_doc\n");
    tmp_file = __get_name(cmd->index);
    fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 00644);
    __putstr_fd(cmd->redirect->file_name, fd);
    close(fd);
    cmd->redirection[0] = open(tmp_file, O_RDONLY);
    if (cmd->redirection[0] < 0)
        __exit_child(seq, cmd, errno, 1);
    if (dup2(cmd->redirection[0], 0) < 0)
        __exit_child(seq, cmd, errno, 1);
    if (close(cmd->redirection[0]) < 0)
        __exit_child(seq, cmd, errno, 1);
    //creer le fichier
    //faire la redirection vers le open du ficher
    // count_hd ++ dans le seq
}

int __handle_redirect(t_sequ *seq, t_cmd *cmd)
{
    DEBUG && fprintf(stderr,"Handle redirect\n");
    while(cmd->redirect)
    {
        if (cmd->redirect->type ==  LESS)
            __less_redirection(seq, cmd);
        if (cmd->redirect->type ==  GREAT)
            __great_redirection(seq, cmd);
        if (cmd->redirect->type ==  DGREAT)
            __dgreat_redirection(seq, cmd);
        if (cmd->redirect->type == H_D)
            __here_doc_redirection(seq, cmd);
        cmd->redirect = cmd->redirect->next;
    }
    return (1);
}