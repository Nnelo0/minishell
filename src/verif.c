/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 08:36:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/21 08:55:13 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	has_caracter_in_quotes(char *str, int limiter)
{
	char	quote;
	char	*start;
	char	*end;

	while (*str)
	{
		if (*str == '\"' || *str == '\'')
		{
			quote = *str;
			start = str + 1;
			end = ft_strchr(start, quote);
			if (!end)
				return (0);
			if (ft_strchr(start, limiter) && ft_strchr(start, limiter) < end)
				return (2);
			str = end;
		}
		str++;
	}
	return (0);
}

int	valid_pipe(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->input[i])
	{
		if (has_caracter_in_quotes(shell->input[i], '|'))
			return (2);
		if (ft_strchr(shell->input[i], '|'))
		{
			if (ft_strlen(shell->input[i]) != 1
				|| !shell->input[i + 1] || i == 0)
				return (1);
		}
		i++;
	}
	return (0);
}

int	valid_redirection(char **input)
{
	int	i;
	int	j;
	int	valid_cmd;

	i = 0;
	valid_cmd = 0;
	while (input[i])
	{
		if (has_caracter_in_quotes(input[i], '>')
			|| has_caracter_in_quotes(input[i], '<'))
			return (2);
		if (ft_strchr(input[i], '>') || ft_strchr(input[i], '<'))
		{
			j = 0;
			if (input[i][j + 1] == input[i][j]
				&& input[i][j + 2] == input[i][j])
				valid_cmd = 1;
			if (input[i][j + 1] != input[i][j])
				valid_cmd = 1;
			if (!input[i + 1])
				valid_cmd = 1;
		}
		i++;
	}
	return (valid_cmd);
}

int	verif_shell(char *input, t_shell *shell, int type)
{
	if (input[0] == '$' && !input[1])
		return (input = NULL, ft_putstr_fd("$: command not found\n", 2)
			, shell->status = 127, type = 1);
	if ((input[0] == '\'' && input[1] == '\'' )
		|| (input[0] == '"' && input[1] == '"'))
		return (input = NULL, ft_putstr_fd("Command '' not found\n", 2)
			, shell->status = 127, type = 1);
	if (input[0] == '.' && !input[1])
		return (ft_putstr_fd(".: filename argument required\n\
.: usage: . filename [arguments]\n", 2), 2);
	if (input[0] == '/' && !input[1])
		return (input = NULL, ft_putstr_fd("/: Is a directory\n", 2)
			, shell->status = 126, type = 1);
	return (type = 0);
}
