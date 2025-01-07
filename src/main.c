/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/07 09:25:01 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\nminishell> ", 12);
}

int	keypress(char *input, t_shell *shell)
{
	if (shell->signal_status == 1)
	{
		shell->signal_status = 0;
		if (input)
			free(input);
		return (1);
	}
	if (shell->signal_status == 2)
	{
		shell->signal_status = 0;
		return (1);
	}
	if (!input)
	{
		printf("exit\n");
		return (0);
	}
	return (1);
}

void	handle_prompt(t_shell *shell)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!keypress(input, shell))
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
	t_shell	shell;

	shell.signal_status = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	handle_prompt(&shell);
	return (0);
}
