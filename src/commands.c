/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/06 15:35:58 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_yes(void)
{
	while (1)
		printf("y\n");
}

int	ft_exit(char *input)
{
	free(input);
	return (0);
}

int	commands(char *input)
{
	if (ft_strncmp(input, "exit", 4) == 0 && (input[4] == ' ' || input[4] == '\0'))
			return(ft_exit(input));
	if (ft_strcmp(input, "yes") == 0)
		ft_yes();
	return (1);
}