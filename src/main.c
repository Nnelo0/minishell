/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 10:16:59 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/10 12:46:57 by cle-berr         ###   ########.fr       */
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

t_env	*create_env_node(const char *env_var)
{
	t_env	*new_node;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->value = strdup(env_var);
	if (!new_node->value)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

void	append_env_node(t_env **head, const char *env_var)
{
	t_env	*new_node;
	t_env	*temp;

	new_node = create_env_node(env_var);
	if (!new_node)
		return ;
	if (*head == NULL)
	{
		*head = new_node;
	}
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_node;
	}
}

t_env	*init_env_list(char **env)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		append_env_node(&env_list, env[i]);
		i++;
	}
	return (env_list);
}

void	free_env_list(t_env *env_list)
{
	t_env	*temp;

	while (env_list)
	{
		temp = env_list->next;
		free(env_list->value);
		free(env_list);
		env_list = temp;
	}
}


int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell.env_list = init_env_list(envp);
	shell.signal_status = 0;
	shell.args = NULL;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	handle_prompt(&shell, envp);
	free_env_list(shell.env_list);
	return (0);
}
