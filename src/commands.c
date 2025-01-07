/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/07 08:52:08 by ebroudic         ###   ########.fr       */
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
int		ft_quotes(char *input)
{
	int	i;
	int	j;

	i = 0;
	while (input[i])
	{
		if (input[i] == 34)
		{
			j = i;
			while (input[j])
			{
				if (input[i] == 34)
					return (1);
				j++;
			}
			return (0);
		}
		if (input[i] == 39)
		{
			j = i;
			while (input[j])
			{
				if (input[i] == 39)
					return (1);
				j++;
			}
			return (0);
		}
		i++;
	}
	return (1);
}

void	ft_echo(char *input)
{
	int	i;

	if (input[5] == '-' && input[6] == 'n' && (input[7] == ' ' || input[7] == '\0'))
	{
		i = 8;
		while (input[i])
		{
			while (input[i] == 34 || input[i] == 39)
				i++;
			write(1, &input[i], 1);
			i++;
		}
		return ;
	}
	i = 5;
	while (input[i])
		{
			while (input[i] == 34 || input[i] == 39)
				i++;
			write(1, &input[i], 1);
			i++;
		}
	ft_printf("\n");
}

int	commands(char *input)
{
	if (!ft_quotes(input))
		printf
	if (ft_strncmp(input, "exit", 4) == 0 && (input[4] == ' ' || input[4] == '\0'))
			return(ft_exit(input));
	if (ft_strcmp(input, "yes") == 0)
		ft_yes();
	if (ft_strncmp(input, "echo", 4) == 0 && (input[4] == ' ' || input[4] == '\0'))
		ft_echo(input);
	return (1);
}
