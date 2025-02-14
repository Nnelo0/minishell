/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 08:36:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/14 19:43:11 by nnelo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_valid_pipe(char *input)
{
	int	i;
	int	valid_command;
	int	quote;

	i = 0;
	valid_command = 0;
	while (input[i])
	{
		while (input[i] != '\0')
		{
			if (input[i] == 39 || input[i] == 34)
				quote = !quote;
			if (quote == 1)
				break ;
			i++;
		}
		if (input[i] == '|')
		{
			if (i == 0 || input[i + 1] == '|' || input[i + 1] == '\0')
				return (0);
			if (!valid_command)
				return (0);
			valid_command = 0;
		}
		else if (input[i] != ' ')
			valid_command = 1;
		i++;
	}
	return (valid_command);
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
