/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/07 09:34:31 by ebroudic         ###   ########.fr       */
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
	printf("exit\n");
	free(input);
	return (0);
}

int	ft_quotes(char *input)
{
	int	i;
	int	single_open;
	int	double_open;

	i = 0;
	single_open = 0;
	double_open = 0;
	while (input[i])
	{
		if (input[i] == 39)
			single_open = !single_open;
		else if (input[i] == 34)
			double_open = !double_open;
		i++;
	}
	if (single_open || double_open)
		return (0);
	return (1);
}

void	ft_echo(char *input)
{
	int	i;

	if (input[5] == '-' && input[6] == 'n'
		&& (input[7] == ' ' || input[7] == '\0'))
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
		ft_printf("open quote\n");
	if (ft_strncmp(input, "exit", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_exit(input));
	if (ft_strcmp(input, "yes") == 0)
		ft_yes();
	if (ft_strncmp(input, "echo", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		ft_echo(input);
	return (1);
}
