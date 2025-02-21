/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 08:36:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/21 10:43:32 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	valid_pipe(char *input, int i)
{
	if (ft_strchr(input, '\"') || (ft_strchr(input, '\'')))
		return (2);
	if (ft_strchr(input, '|'))
	{
		if (ft_strlen(input) != 1 || i == 0)
			return (1);
	}
	return (0);
}

int	valid_redirection(char *input)
{
	if (ft_strchr(input, '\"') || (ft_strchr(input, '\'')))
		return (2);
	return (0);
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
