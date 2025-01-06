/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/06 15:26:33 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\nminishell> ", 12);
}

void	handle_sigquit(int sig)
{
	(void)sig;
	write(1, "minishell> ", 12);
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
		return (0);
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
		if (strcmp(input, "exit") == 0)
		{
			free(input);
			break ;
		}
		if (strcmp(input, "yes") == 0)
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
	t_shell	shell;

	shell.signal_status = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	handle_prompt(&shell);
	return (0);
}

