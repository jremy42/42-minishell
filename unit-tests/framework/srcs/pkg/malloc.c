/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   __putnbr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngiroux <ngiroux@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/09 10:24:09 by ngiroux           #+#    #+#             */
/*   Updated: 2022/01/09 15:05:55 by ngiroux          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libunit.h"


int __leaks(int fd)
{
    char buffer[BUFFER_SIZE];
    char **search;
    int i;

    i = 0;
    read(fd,buffer, BUFFER_SIZE);
    search = __split(buffer, ' ');
    while(search[i] != NULL)
    {
        if (strcmp(search[i],"All") == 0)
        {
            i++;
            if (strcmp(search[i],"heap") == 0)
            {
                i++;
                if (strcmp(search[i],"blocks") == 0)
                {
                    i++;
                    if (strcmp(search[i],"were") == 0)
                    {
                        i++;
                        if (strcmp(search[i],"freed") == 0)
                            return (0);
                    }
                }
            }
        }
        i++;
    }
    return (69);
}