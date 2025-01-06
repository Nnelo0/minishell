/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/06 13:51:39 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	keypress(char *input)
{
	if (!input)
		return (0);
	return (1);
}

void	handle_prompt(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!keypress(input))
			break ;
		if (ft_strncmp(input, "exit", 4) == 0 && (input[4] == ' ' || input[4] == '\0'))
		{
			free(input);
			break ;
		}
		if (ft_strcmp(input, "yes") == 0)
		{
			while (1)
				printf("y\n");
		}
		if (*input)
			add_history(input);
		free(input);
	}
}

int	main(void)
{
	handle_prompt();
	return (0);
}
