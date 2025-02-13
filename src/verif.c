/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 08:36:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/13 08:36:54 by ebroudic         ###   ########.fr       */
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
