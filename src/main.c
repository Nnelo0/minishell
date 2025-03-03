/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/03 12:48:57 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		g_status = 0;

void	handle_sigint(int sig)
{
	(void)sig;
	g_status = 130;
	if (waitpid(-1, NULL, WNOHANG) == 0)
	{
		ft_printf("\n");
		return ;
	}
	ft_printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
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

void	handle_prompt(t_shell *shell)
{
	char	*input;

	while (42)
	{
		input = readline("minishell> ");
		keypress(input, shell);
		if (*input)
			add_history(input);
		shell->status = commands(input, shell, &g_status);
		if (input)
			free(input);
	}
}
void	handle_signal(void (*f)(int))
{
	signal(SIGINT, (*f));
	signal(SIGQUIT, SIG_IGN);
}
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.status = g_status;
	shell.fd_out = -1;
	shell.fd_in = -1;
	shell.prev_fd = -1;
	shell.save_in = -1;
	shell.save_out = -1;
	shell.env_list = init_env_list(envp);
	shell.export_list = init_export_list(envp);
	shell.env = NULL;
	handle_signal(handle_sigint);
	handle_prompt(&shell);
	free_env_list(shell.env_list);
	free_export_list(shell.export_list);
	return (0);
}
