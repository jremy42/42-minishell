/* ************************************************************************** */
/**/
/*:::  ::::::::   */
/*   __strncmp.c   :+:  :+::+:   */
/*+:+ +:+ +:+ */
/*   By: jremy <jremy@student.42.fr>+#+  +:+   +#+*/
/*+#+#+#+#+#+   +#+   */
/*   Created: 2021/11/23 17:22:55 by jremy #+##+# */
/*   Updated: 2022/01/10 15:04:51 by jremy###   ########.fr   */
/**/
/* ************************************************************************** */

#include "libft.h"

int	__strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' || s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}

int	__strcmp_ignore_case(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (__tolower(s1[i]) != __tolower(s2[i]))
			return (__tolower(((unsigned char *)s1)[i])
				- __tolower(((unsigned char *)s2)[i]));
		i++;
	}
	return (__tolower(((unsigned char *)s1)[i])
		- __tolower(((unsigned char *)s2)[i]));
}
