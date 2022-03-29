/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jremy <jremy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 14:53:22 by jremy             #+#    #+#             */
/*   Updated: 2022/03/29 18:19:48 by jremy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "exe.h"


int	__treat_last_rv(char **expanded_token, int *i, t_msh *msh)
{
	char *candidate;

	candidate = __itoa(msh->rv);
	if (!candidate)
		return (0);
	*i += 1;
	*expanded_token = __strjoin(*expanded_token, candidate);
	if(!*expanded_token)
		return(free(candidate), 0);
	return (free(candidate), 1);
}

char *__get_candidate(char *start_word, int *i)
{
	char *candidate;
	int	j;

	j = 0;
	while (__isalnum(start_word[j]) || start_word[j] == '_')
		j++;
	candidate = __substr(start_word, 0, j);
	if(!candidate)
		return(NULL);
	*i += j;
	return (candidate);
}

int	__substitute_candidate(char *env_key, char *candidate, char **expanded_token, char *env_value)
{
	free(env_key);
	free(candidate);
	*expanded_token =__strjoin(*expanded_token, env_value);
	if(!*expanded_token)
		return(0);
	return(1);
}

int __parameter_expand(char *start_word, t_msh *msh, char **expanded_token, int *i)
{
	char *candidate;
	char *env_key;
	int	j;

	if (start_word[0] == '?')
		return(__treat_last_rv(expanded_token, i, msh));
	candidate = __get_candidate(start_word, i);
	if (!candidate)
		return (0);
	j = 0;
	while(msh->envp[j])
	{
		if (msh->envp[j][1][0] == '1')
		{
			env_key = __substr(msh->envp[j][0], 0, __strchr(msh->envp[j][0], '=') - msh->envp[j][0]);
			if(!env_key)
				return(free(candidate), 0);
			if (!__strcmp(candidate, env_key) && msh->envp[j][1] && __strchr(msh->envp[j][0], '='))
				return(__substitute_candidate(env_key, candidate, expanded_token, __strchr(msh->envp[j][0], '=') + 1 ));
			free(env_key);
		}
		j++;
	}
	return (free(candidate), 1);
}

int	__treat_dollar(char c, char next_char, t_state quote_status)
{
	if (c == '$' && next_char && (quote_status == UNQUOTE || quote_status == D_QUOTE))
		return (1);
	return (0);
}

int __expand_word(char **token_word, t_msh  *msh)
{
	t_state slash_status;
	t_state quote_status;
	char *tmp;
	int     i;
	char    *expanded_token_word;

	expanded_token_word = __strdup("");
	if (!expanded_token_word)
		return (0);
	tmp = *token_word;
	quote_status = UNQUOTE;
	slash_status = 0;
	i = 0;
	while(tmp[i])
	{
		if (tmp[i] == '\\' && tmp[i + 1] && __need_to_escape(i, quote_status, tmp)
			&& !slash_status)
		{
			slash_status = BACKSLASH;
			if (!__add_char_to_token(tmp[i], &expanded_token_word))
				return(free(expanded_token_word), 0);
			i++;
			continue ;
		}
		quote_status = __return_state(tmp[i], quote_status, slash_status);		
		if(!slash_status && __treat_dollar(tmp[i], tmp[i + 1], quote_status))
		{
			if(!__parameter_expand(tmp + i + 1, msh, &expanded_token_word, &i))
				return (free(expanded_token_word), 0);
		}
		else
		{
			if (!__add_char_to_token(tmp[i], &expanded_token_word))
				return(free(expanded_token_word), 0);
		}
		i++;
		slash_status = 0;
	}
	free(*token_word);
	*token_word = expanded_token_word;
	return (1);
}

int	__split_expanded_token(t_lexing *lexing)
{
	char	**split_token;
	int		i;

	i = 0;

	DEBUG && printf("lexing->token = %s\n", lexing->token);
	split_token = __split(lexing->token, ' ');
	if (!split_token)
		return (0);
	while (split_token[i])	
	{
		if (!__insert_token(lexing, split_token[i], 0, NULL))
			return (free_split(split_token), 0);
		i++;
	}
	free_split(split_token);
	__insert_token(NULL, NULL, 1, NULL); //reset static 
	//free(split_token);
	return (1);
}

int __parameter_expand_token(t_lexing *lexing, t_msh *msh)
{
	while(lexing)
	{
		if(lexing->type == WORD)
		{
						DEBUG && fprintf(stderr, "new_token before parameter expand: [%s]\n", lexing->token);
			if(!__expand_word(&lexing->token, msh))
				return (__putendl_fd("Malloc error", 2), 0);
			DEBUG && fprintf(stderr, "new_token after parameter expand: [%s]\n", lexing->token);
		}
		lexing = lexing->next;
	}
	return (1);
}

//passer lexing a expand word;
//si il y a un espace dans le treat dollar > cut au premiere espace et creer un autre token a la suite
/*
if (ret == 2 && previous->type != REDIRECTION)
				if (!__split_expanded_token(lexing))
					return (__putendl_fd("Malloc error", 2), 0);
					*/
