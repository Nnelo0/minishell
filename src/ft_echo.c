/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 16:17:37 by nnelo             #+#    #+#             */
/*   Updated: 2025/02/04 13:45:27 by cle-berr         ###   ########.fr       */
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

void	ft_remove_quotes(char *input)
{
	int		i;
	int		j;
	char	quote_type;

	i = 0;
	j = 0;
	quote_type = '\0';
	while (input[i])
	{
		if ((input[i] == 39 || input[i] == 34) && quote_type == '\0')
			quote_type = input[i];
		else if (input[i] == quote_type)
			quote_type = '\0';
		else
		{
			input[j] = input[i];
			j++;
		}
		i++;
	}
	if (quote_type != '\0')
		input[++j] = quote_type;
	input[j] = '\0';
}

int	ft_echo(char *input)
{
	int	i;
	char	**args;

	i = 1;
	args = ft_split_quote(input, ' ');
	if (ft_strcmp(args[1], "-n") == 0)
		while (args[++i])
			printf("%s", args[i]);
	return (1);
	/* if (ft_strlen(input) >= 5 && input[4] == ' ')
	{
		if (input[5] == '-' && input[6] == 'n'
			&& (input[7] == ' ' || input[7] == '\0'))
		{
			i = 7;
			while (input[i] == ' ')
				i++;
			if (input[i] == '\0')
				return (1);
			ft_printf("%s", input + i);
			return (1);
		}
		i = 5;
		ft_printf("%s", input + i);
	}
	ft_printf("\n");
	return (1); */
}
