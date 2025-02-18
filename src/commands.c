/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/18 10:57:54 by cle-berr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char **input, t_shell *shell)
{
	printf("exit\n");
	if (input[1])
	{
		if (ft_isdigit_s(input[1]))
		{
			shell->status = ft_atoi(input[1]);
			if (input[2])
				return (shell->status = 1, free_args(input),
					printf("too many arguments\n"), shell->status);
		}
		if (!ft_isdigit_s(input[1]))
		{
			shell->status = 2;
			printf("%s: numeric argument required\n", input[1]);
		}
		else if (ft_strlen(input[1]) >= 20)
		{
			shell->status = 2;
			printf("%s: numeric argument required\n", input[1]);
		}
	}
	return (free_all(shell, input), rl_clear_history(),
		exit(shell->status), shell->status);
}

int	ft_exe(char **args, char **envp, t_shell *shell)
{
	pid_t		pid;
	int			status;
	struct stat	path_stat;

	if (!args || !args[0])
		return (printf("command not found\n"));
	if (stat(args[0], &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (printf("%s: Is a directory\n", args[0]),
				free_args(args), 126);
		if (S_ISREG(path_stat.st_mode) && access(args[0], X_OK) == -1)
			return (printf("%s: Permission denied\n", args[0]),
				free_args(args), 126);
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		ft_execute(args, envp, shell);
	return (waitpid(pid, &status, 0), free_args(args), (status >> 8) & 0xFF);
}

int	ft_cd(char **args)
{
	char	*target;

	if (!args[1])
	{
		target = getenv("HOME");
		if (!target)
			return (printf("HOME not set\n"), 1);
	}
	else if (args[2])
		return (printf("too many arguments\n"), 1);
	else
		target = args[1];
	if (chdir(target) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	which_commands(char **input, char **envp, t_shell *shell)
{
	if (ft_strncmp(input[0], "exit", 4) == 0
		&& (input[0][4] == ' ' || input[0][4] == '\0'))
		return (ft_exit(input, shell));
	if (ft_strncmp(input[0], "echo", 4) == 0
		&& (input[0][4] == ' ' || input[0][4] == '\0'))
		return (ft_echo(input, shell, 0, 1));
	if (ft_strncmp(input[0], "cd", 2) == 0
		&& (input[0][2] == ' ' || input[0][2] == '\0'))
		return (ft_cd(input));
	if (ft_strncmp(input[0], "./", 2) == 0)
		return (ft_exe(input, envp, shell));
	if (ft_strncmp(input[0], "env", 3) == 0
		&& (input[0][3] == ' ' || input[0][3] == '\0'))
		return (ft_env(shell));
	if (ft_strncmp(input[0], "pwd", 3) == 0 && input[0][3] == '\0')
		return (ft_pwd());
	if (ft_strncmp(input[0], "export", 6) == 0
		&& (input[0][6] == ' ' || input[0][6] == '\0'))
		return (ft_export(shell));
	if (ft_strncmp(input[0], "unset", 5) == 0
		&& (input[0][5] == ' ' || input[0][5] == '\0'))
		return (ft_unset(shell));
	return (ft_shell(input, envp, shell, 0));
}

int	commands(char *input, char **envp, t_shell *shell, int *status)
{
	(void)envp;
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (0);
	if (*status == 130)
	{
		shell->status = 130;
		*status = 0;
	}
	if (!ft_quotes(input))
		return (ft_printf("open quote\n"), 127);
	shell->test = ft_split_quote(input, ' ');
	shell->test[0] = get_command_from_path(shell->test[0]);
	ft_remove_quotes(shell->test[0]);
	shell->input = ft_strdup(input);
	shell->cmd = NULL;
	shell->ipt = NULL;
	shell->env = env_in_stars(shell);
	shell->status = which_commands(shell->test, shell->env, shell);
	free(shell->input);
	free_args(shell->env);
	return (shell->status);
}
