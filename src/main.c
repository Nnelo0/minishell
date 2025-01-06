/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/06 15:36:25 by ebroudic         ###   ########.fr       */
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
		if (!commands(input))
			break ;
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
