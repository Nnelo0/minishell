/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/31 10:22:38 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char *input, t_shell *shell)
{
	ft_printf("exit\n");
	free(input);
	free_args(shell->args);
	rl_clear_history();
	return (0);
}

int	ft_exe(t_shell *shell, char **envp)
{
	pid_t	pid;

	if (!shell->args || !shell->args[0])
		return (printf("minishell: command not found\n"));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		if (execve(shell->args[0], shell->args, envp) == -1)
		{
			perror("minishell");
			free_args(shell->args);
			exit(1);
		}
	}
	wait(NULL);
	free_args(shell->args);
	shell->args = NULL;
	return (1);
}

int	ft_cd(t_shell *shell)
{
	char	*target;

	if (!shell->args[1])
	{
		target = getenv("HOME");
		if (!target)
			return (printf("cd: HOME not set\n"));
	}
	else if (shell->args[2])
		return (printf("cd: too many arguments\n"));
	else
		target = shell->args[1];
	if (chdir(target) == -1)
	{
		perror("cd");
		return (1);
	}
	return (1);
}

int	which_commands(char *input, char **envp, t_shell *shell)
{
	if (ft_strchr(input, '|'))
		return (ft_pipe(input, envp, shell));
	if (ft_strchr(input, '<') || ft_strchr(input, '>'))
		return (ft_redirection(shell));
	if (ft_strncmp(input, "exit", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_exit(input, shell));
	if (ft_strncmp(input, "echo", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_echo(input));
	if (ft_strncmp(input, "cd", 2) == 0
		&& (input[2] == ' ' || input[2] == '\0'))
		return (ft_cd(shell));
	if (ft_strncmp(input, "./", 2) == 0)
		return (ft_exe(shell, envp));
	if (ft_strncmp(input, "env", 3) == 0
		&& (input[3] == ' ' || input[3] == '\0'))
		return (ft_env(shell));
	if (ft_strncmp(input, "pwd", 3) == 0 && input[3] == '\0')
		return (ft_pwd());
	if (ft_strncmp(input, "export", 6) == 0
		&& (input[6] == ' ' || input[6] == '\0'))
		return (ft_export(shell));
	if (ft_strncmp(input, "unset", 5) == 0
		&& (input[5] == ' ' || input[5] == '\0'))
		return (ft_unset(shell));
	return (ft_shell(input, envp, shell));
}

int	commands(char *input, char **envp, t_shell *shell)
{
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (1);
	shell->input = ft_strdup(input);
	shell->args = ft_split(input, ' ');
	shell->cmd = NULL;
	shell->ipt = NULL;
	if (!ft_quotes(input))
		return (ft_printf("open quote\n"));
	ft_remove_quotes(input);
	if (!is_valid_chevrons(input))
		return (ft_printf("invalid '<'\n"));
	if (!is_valid_pipe(input))
		return (ft_printf("invalid pipes\n"));
	shell->ipt = ft_split_chevrons(input, -1, 0);
	if (!shell->ipt)
		return (write(2, "ft_split failed\n", 16), 1);
	return (which_commands(input, envp, shell), free(shell->input), 1);
}
