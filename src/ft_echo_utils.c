/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 15:20:44 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/27 17:20:00 by ebroudic         ###   ########.fr       */
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
	if (!ft_quotes(input))
		return ;
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

char	*ft_add_quotes(char *input, int i)
{
	char	*tmp;

	if (!ft_quotes(input))
		return (input);
	while (input[i++])
	{
		if (input[i] == '\"')
			break ;
		if (input[i] == '\'')
			break ;
	}
	if (input[i] == '\"')
	{
		tmp = ft_strjoin("'", input);
		free(input);
		input = ft_strjoin(tmp, "'");
		return (free(tmp), input);
	}
	else
	{
		tmp = ft_strjoin("\"", input);
		free(input);
		input = ft_strjoin(tmp, "\"");
		return (free(tmp), input);
	}
}
