/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/11 09:41:36 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char *input, t_shell *shell)
{
	shell->exit = ft_split(input, ' ');
	printf("exit\n");
	if (shell->exit[1])
	{
		if (ft_isdigit_s(shell->exit[1]))
		{
			shell->status = ft_atoi(shell->exit[1]);
			if (shell->exit[2])
				return (shell->status = 1,
					printf("exit: too many arguments\n"), shell->status);
		}
		if (!ft_isdigit_s(shell->exit[1]))
		{
			shell->status = 2;
			printf("exit: %s: numeric argument required\n", shell->exit[1]);
		}
		else if (ft_strlen(shell->exit[1]) >= 20)
		{
			shell->status = 2;
			printf("exit: %s: numeric argument required\n", shell->exit[1]);
		}
	}
	return (free_all(shell, input), rl_clear_history(),
		exit(shell->status), shell->status);
}

int	ft_exe(char *input, char **envp, t_shell *shell)
{
	pid_t		pid;
	char		**args;
	int			status;
	struct stat	path_stat;

	args = ft_split(input, ' ');
	if (!args || !args[0])
		return (printf("minishell: command not found\n"));
	if (stat(args[0], &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (printf("minishell: %s: Is a directory\n", args[0]),
				free_args(args), 126);
		if (S_ISREG(path_stat.st_mode) && access(args[0], X_OK) == -1)
			return (printf("minishell: %s: Permission denied\n", args[0]),
				free_args(args), 126);
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		ft_execute(args, envp, shell);
	return (waitpid(pid, &status, 0), free_args(args), (status >> 8) & 0xFF);
}

int	ft_cd(char *input)
{
	char	*target;
	char	**args;

	args = ft_split(input, ' ');
	if (!args[1])
	{
		target = getenv("HOME");
		if (!target)
			return (printf("cd: HOME not set\n"), 1);
	}
	else if (args[2])
		return (printf("cd: too many arguments\n"), 1);
	else
		target = args[1];
	if (chdir(target) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	which_commands(char *input, char **envp, t_shell *shell)
{
	if (ft_strchr(input, '|'))
		return (ft_pipe(input, envp, shell));
	if (ft_strchr(input, '<') || ft_strchr(input, '>'))
		return (ft_redirection(input, shell));
	if (ft_strncmp(input, "exit", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_exit(input, shell));
	if (ft_strncmp(input, "echo", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_echo(input, shell, 0, 1));
	if (ft_strncmp(input, "cd", 2) == 0
		&& (input[2] == ' ' || input[2] == '\0'))
		return (ft_cd(input));
	if (ft_strncmp(input, "./", 2) == 0)
		return (ft_exe(input, envp, shell));
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
	int	res;

	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (0);
	shell->input = ft_strdup(input);
	shell->cmd = NULL;
	shell->ipt = NULL;
	if (!ft_quotes(input))
		return (free(shell->input), ft_printf("open quote\n"), 127);
	if (!is_valid_chevrons(input))
		return (free(shell->input), ft_printf("invalid '<' or '>'\n"), 127);
	if (!is_valid_pipe(input))
		return (free(shell->input), ft_printf("invalid pipes\n"), 127);
	shell->ipt = ft_split_chevrons(input, -1, 0);
	res = which_commands(input, envp, shell);
	free(shell->input);
	return (res);
}
