/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/11 10:48:20 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		status = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	if (waitpid(-1, NULL, WNOHANG) == 0)
	{
		ft_printf("\n");
		return ;
	}
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	status = 130;
}

int	keypress(char *input, t_shell *shell)
{
	if (shell->signal_status == 1)
	{
		shell->signal_status = 0;
		if (input)
			free(input);
		return (0);
	}
	if (shell->signal_status == 2)
	{
		shell->signal_status = 0;
		return (0);
	}
	if (!input)
	{
		printf("exit\n");
		rl_clear_history();
		free(input);
		free_args(shell->args);
		free_args(shell->ipt);
		free_env_list(shell->env_list);
		free_export_list(shell->export_list);
		exit (shell->status);
		return (shell->status);
	}
	return (0);
}

void	handle_prompt(t_shell *shell, char **envp)
{
	char	*input;

	while (1)
	{
		input = readline("minishell> ");
		keypress(input, shell);
		if (*input)
			add_history(input);
		shell->status = commands(input, envp, shell, &status);
		if (shell->args)
		{
			free_args(shell->args);
			shell->args = NULL;
		}
		if (shell->ipt)
		{
			free_args(shell->ipt);
			shell->ipt = NULL;
		}
		if (input)
			free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.status = status;
	shell.signal_status = 0;
	shell.args = NULL;
	shell.envp1 = envp;
	shell.ipt = NULL;
	shell.fd_out = -1;
	shell.fd_in = -1;
	shell.env_list = init_env_list(envp);
	shell.export_list = init_export_list(envp);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	handle_prompt(&shell, envp);
	free_env_list(shell.env_list);
	free_export_list(shell.export_list);
	return (0);
}
