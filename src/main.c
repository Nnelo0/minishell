/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/09 16:47:38 by cle-berr         ###   ########.fr       */
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
		{
			free_args(shell->args);
			shell->args = NULL;
		}
		free(input);
	}
}

t_list	*copy_env(char **envp)
{
	t_list	*new_list;
	int		i;

	i = 0;
	while (envp[i])
	{
		new_list = malloc(sizeof(t_list));
		if (!new_list)
		{
			ft_lstclear(&new_list);
			return (NULL);
		}
		new_list->content = envp[i];
		new_list->next = NULL;
		ft_lstadd_back(&new_list, ft_strdup(envp[i]));
	}
	return (&new_list);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;
	t_list	*list_env;

	shell.env = copy_env();
	(void)argc;
	(void)argv;
	shell.signal_status = 0;
	shell.args = NULL;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	handle_prompt(&shell, envp);
	return (0);
}
