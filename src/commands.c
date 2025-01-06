/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/06 15:55:59 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_yes(char *input, t_shell *shell)
{
	while (1)
	{
		printf("y\n");
		if (!keypress(input, shell))
			break ;
	}
}

int	ft_exit(char *input)
{
	printf("exit\n");
	free(input);
	return (0);
}

int	commands(char *input, t_shell *shell)
{
	if (ft_strncmp(input, "exit", 4) == 0 && \
		(input[4] == ' ' || input[4] == '\0'))
		return (ft_exit(input));
	if (ft_strcmp(input, "yes") == 0)
		ft_yes(input, shell);
	return (1);
}