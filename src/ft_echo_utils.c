/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:20:44 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/13 16:35:37 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
			ft_printf("(%d)\n", quote_type);
			ft_printf("[%s]\n", input);
			input[j] = input[i];
			ft_printf("{%s}\n", input);
			j++;
		}
		i++;
	}
	if (quote_type != '\0')
		input[++j] = quote_type;
	input[j] = '\0';
	ft_printf("---%s---\n\n", input);
}
