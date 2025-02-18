/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 08:36:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/17 16:53:47 by nnelo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	valid_pipe(t_shell *shell)
{
	int	i;
	int	valid_cmd;

	i = 0;
	valid_cmd = 0;
	while (shell->input[i])
	{
		if (ft_strchr(shell->input[i], '\"'))
			return (2);
		if (ft_strchr(shell->input[i], '|'))
		{
			if (ft_strlen(shell->input[i]) != 1)
				valid_cmd = 1;
			if (!shell->input[i + 1])
				valid_cmd = 1;
			if (!shell->input[i - 1])
				valid_cmd = 1;
		}
		i++;
	}
	return (valid_cmd);
}

int	verif_shell(char *input, t_shell *shell)
{
	if ((input[0] == '\'' && input[1] == '\'' )
		|| (input[0] == '"' && input[1] == '"'))
		return (ft_printf("Command '' not found\n"), close(shell->in),
			close(shell->out), 127);
	if (input[0] == '.' && !input[1])
		return (ft_printf(".: filename argument required\n\
.: usage: . filename [arguments]\n"), close(shell->in), close(shell->out), 2);
	if (input[0] == '/' && !input[1])
		return (ft_printf("/: Is a directory\n"), close(shell->in),
			close(shell->out), 126);
	return (0);
}
