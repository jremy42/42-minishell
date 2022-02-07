/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/07 17:25:57 by jremy             #+#    #+#             */
/*   Updated: 2022/02/07 19:06:08 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main ()
{
    char *arg[10];
    int i;
    char *prompt;

    i = 0;
    prompt = NULL;
    while (1)
    {
        prompt = readline("prout~");
        arg[i] = prompt;
        if(prompt[0] == '*')
            break ;
        i++;
    }
    arg[i++] = NULL;
    __pwd(1);
    __echo(arg, 1);

}