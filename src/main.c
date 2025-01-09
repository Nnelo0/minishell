/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/09 10:42:36 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_sigint(int sig)
{
	(void)sig;
	if (waitpid(-1, NULL, WNOHANG) == 0)
	{
		ft_printf("\n");
		return ;
	}
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

void	handle_prompt(t_shell *shell, char **envp)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		if (!keypress(input, shell))
			break ;
		if (*input)
			add_history(input);
		if (!commands(input, envp, shell))
			break ;
		if (shell->args)
			free_args(shell->args);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.signal_status = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	handle_prompt(&shell, envp);
	return (0);
}
