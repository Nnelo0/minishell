/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:20:44 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/26 13:30:05 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
		if (input[i] == 39 && double_open == 0)
			single_open = !single_open;
		else if (input[i] == 34 && single_open == 0)
			double_open = !double_open;
		i++;
	}
	if (single_open || double_open)
		return (0);
	return (1);
}

void	echo_utils(char **args, int i, t_shell *shell, int n)
{
	if (ft_strcmp(args[i], "-n") == 0 && n != 2)
		n = 1;
	else if ((i == 1 && n != 1) || i != 1)
	{
		printf("%s", args[i]);
		n = 2;
	}
	if (args[i + 1])
	{
		if (ft_strcmp(args[i], "-n") != 0 || n == 2)
			printf(" ");
		echo_utils(args, i + 1, shell, n);
	}
	if (n != 1 && i == 1)
		printf("\n");
}

int	ft_echo(char **args, t_shell *shell, int n, int i)
{
	if (!args[1])
		printf("\n");
	else
		echo_utils(args, i, shell, n);
	return (0);
}
