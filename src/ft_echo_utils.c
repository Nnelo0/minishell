/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:20:44 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/27 09:23:02 by ebroudic         ###   ########.fr       */
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
		if (input[i] == quote_type)
			quote_type = '\0';
		else if ((input[i] == 39 || input[i] == 34) && quote_type == '\0')
			quote_type = input[i];
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

char	*ft_add_quotes(char *input)
{
	char	*tmp;

	tmp = ft_strjoin("\"", input);
	free(input);
	input = ft_strjoin(tmp, "\"");
	free(tmp);
	return (input);
}
