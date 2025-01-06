/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/06 13:16:45 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_prompt(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (ft_strcmp(input, "exit") == 0)
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
