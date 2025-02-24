/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/25 15:23:18 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_status = 0;

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
	g_status = 130;
}

int	keypress(char *input, t_shell *shell)
{
	if (!input)
	{
		if (g_status == 130)
		{
			shell->status = 130;
			g_status = 0;
		}
		printf("exit\n");
		rl_clear_history();
		free_args(shell->env);
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

	while (WELOVESHELDON)
	{
		input = readline("minishell> ");
		keypress(input, shell);
		if (*input)
			add_history(input);
		shell->status = commands(input, envp, shell, &g_status);
		if (input)
			free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.status = g_status;
	shell.envp1 = envp;
	shell.fd_out = -1;
	shell.fd_in = -1;
	shell.prev_fd = -1;
	shell.save_in = -1;
	shell.save_out = -1;
	shell.env_list = init_env_list(envp);
	shell.export_list = init_export_list(envp);
	shell.env = NULL;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	handle_prompt(&shell, envp);
	free_env_list(shell.env_list);
	free_export_list(shell.export_list);
	return (0);
}
