/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/14 00:50:36 by nnelo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char **input, t_shell *shell)
{
	printf("exit\n");
	ft_remove_quotes(input[1]);
	if (input[1])
	{
		if (ft_isdigit_s(input[1]))
		{
			shell->status = ft_atoi(input[1]);
			if (input[2])
				return (shell->status = 1, free_args(input),
					ft_putstr_fd("too many arguments\n", 2), shell->status);
		}
		if (!ft_isdigit_s(input[1]) || ft_strlen(input[1]) >= 20)
		{
			shell->status = 2;
			ft_printf("%s:", input[1]);
			ft_putstr_fd(" numeric argument required\n", 2);
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
	for (int j = 0; input[j]; j++)
		fprintf(stderr, CYAN "[%s]\n" RESET, input[j]);
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

/* int	verif_input(t_shell *shell)
{
	int	i;

	i = 0;
	while (shell->input[i])
	{
		if (!is_valid_pipe(shell->input[i]))
			return (ft_printf("invalid pipes\n"), 127);
		if (!is_valid_chevrons(shell->input[i]))
			return (ft_printf("invalid '<' or '>'\n"), 127);
		i++;
	}
	return (0);
} */

char	*ft_add_space(const char *input, char c)
{
	int		i;
	int		len;
	char	*result;
	int		in_quotes;

	if (!input)
		return (NULL);
	result = malloc(ft_strlen(input) * 2 + 1);
	if (!result)
		return (NULL);
	i = 0;
	len = 0;
	in_quotes = 0;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			in_quotes = !in_quotes;
		if (!in_quotes && i > 0 && input[i] == c
			&& input[i - 1] != ' ' && input[i - 1] != c)
			result[len++] = ' ';
		result[len++] = input[i];
		if (!in_quotes && input[i] == c
			&& input[i + 1] != ' ' && input[i + 1] != c)
			result[len++] = ' ';
		i++;
	}
	return (result[len] = '\0', result);
}

int	commands(char *input, char **envp, t_shell *shell, int *status)
{
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
	input = ft_add_space(input, '|');
	input = ft_add_space(input, '>');
	input = ft_add_space(input, '<');
	shell->input = ft_split_quote(input, ' ');
	shell->input[0] = get_command_from_path(shell->input[0]);
	ft_remove_quotes(shell->input[0]);
	shell->tmp = ft_strdup(input);
	shell->cmd = NULL;
	shell->ipt = NULL;
	printf(PINK "debut de ft_pipe avec comme input:\n");
	for (int j = 0; shell->input[j]; j++)
		printf("[%s]\n", shell->input[j]);
	shell->status = ft_pipe(envp, shell);
	printf(HOT_PINK "fin de ft_pipe\n" RESET);
	if (shell->input)
		shell->status = which_commands(shell->input, envp, shell);
	free(shell->tmp);
	return (shell->status);
}
