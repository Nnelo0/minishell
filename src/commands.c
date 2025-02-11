/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/11 16:26:33 by cle-berr         ###   ########.fr       */
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
				return (shell->status = 1, free_args(shell->exit),
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
	pid_t	pid;
	char	**args;
	int		status;

	args = ft_split(input, ' ');
	if (!args || !args[0])
		return (printf("minishell: command not found\n"));
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		if (execve(args[0], args, envp) == -1)
		{
			perror(args[0]);
			free_args(shell->ipt);
			free(shell->input);
			free_args(args);
			free_env_list(shell->env_list);
			free_export_list(shell->export_list);
			exit(127);
		}
	}
	waitpid(pid, &status, 0);
	return (free_args(args), args = NULL, (status >> 8) & 0xFF);
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
	if (ft_strchr(shell->verif, '|'))
		return (ft_pipe(input, envp, shell));
	if (ft_strchr(shell->verif, '<') || ft_strchr(shell->verif, '>'))
		return (ft_redirection(input, shell));
	if (ft_strncmp(shell->verif, "exit", 4) == 0
		&& (shell->verif[4] == ' ' || shell->verif[4] == '\0'))
		return (ft_exit(input, shell));
	if (ft_strncmp(shell->verif, "echo", 4) == 0
		&& (shell->verif[4] == ' ' || shell->verif[4] == '\0'))
		return (ft_echo(input, shell, 0, 1));
	if (ft_strncmp(shell->verif, "cd", 2) == 0
		&& (shell->verif[2] == ' ' || shell->verif[2] == '\0'))
		return (ft_cd(input));
	if (ft_strncmp(shell->verif, "./", 2) == 0)
		return (ft_exe(input, envp, shell));
	if (ft_strncmp(shell->verif, "env", 3) == 0
		&& (shell->verif[3] == ' ' || shell->verif[3] == '\0'))
		return (ft_env(shell));
	if (ft_strncmp(shell->verif, "pwd", 3) == 0 && shell->verif[3] == '\0')
		return (ft_pwd());
	if (ft_strncmp(shell->verif, "export", 6) == 0
		&& (shell->verif[6] == ' ' || shell->verif[6] == '\0'))
		return (ft_export(shell));
	if (ft_strncmp(shell->verif, "unset", 5) == 0
		&& (shell->verif[5] == ' ' || shell->verif[5] == '\0'))
		return (ft_unset(shell));
	return (ft_shell(input, envp, shell));
}

int is_executable(const char *path)
{
	return (access(path, X_OK) == 0);
}

char *extract_command_name(const char *path)
{
	char *command_name = strrchr(path, '/');
	if (command_name)
	{
		return (strdup(command_name + 1));
	}
	return (strdup(path));
}

char *find_command_in_path(const char *command, char **envp)
{
	(void)envp;
	char *path = getenv("PATH");
	if (!path)
	{
		return (NULL);
	}

	char *path_copy = strdup(path);
	char *dir = strtok(path_copy, ":");

	while (dir)
	{
		char	*full_path = malloc(strlen(dir) + strlen(command) + 2);
		sprintf(full_path, "%s/%s", dir, command);
		if (is_executable(full_path))
		{
			free(path_copy);
			return (full_path);
		}
		free(full_path);
		dir = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}

char *get_command_name(char *input, char **envp)
{
	if (input[0] == '/')
	{
		if (is_executable(input))
		{
			return (extract_command_name(input));
		}
	}
	else
	{
		char	*full_path = find_command_in_path(input, envp);
		if (full_path)
		{
			char	*command_name = extract_command_name(full_path);
			free(full_path);
			return (command_name);
		}
	}
	return (NULL);
}

int	commands(char *input, char **envp, t_shell *shell)
{
	int		res;
	char	*in;

	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (0);
	if (!ft_quotes(input))
		return (ft_printf("open quote\n"), 127);
	if (!is_valid_chevrons(input))
		return (ft_printf("invalid '<' or '>'\n"), 127);
	if (!is_valid_pipe(input))
		return (ft_printf("invalid pipes\n"), 127);
	shell->verif = ft_strdup(input);
	ft_remove_quotes(shell->verif);
	if (ft_strncmp(shell->verif, "/", 1) == 0)
		in = get_command_name(input, envp);
	else
		in = ft_strdup(input);
	free(shell->verif);
	shell->verif = ft_strdup(input);
	ft_remove_quotes(shell->verif);
	shell->input = ft_strdup(in);
	shell->cmd = NULL;
	shell->ipt = NULL;
	shell->ipt = ft_split_chevrons(in, -1, 0);
	res = which_commands(in, envp, shell);
	free(shell->input);
	free(shell->verif);
	return (res);
}
