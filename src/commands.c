/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 15:01:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/08 19:49:30 by nnelo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char *input)
{
	printf("exit\n");
	free(input);
	return (0);
}

int	ft_quotes(char *input)
{
	int	i;
	int	single_open;
	int	double_open;

	i = 0;
	single_open = 0;
	double_open = 0;
	while (input[i])
	{
		if (input[i] == 39 && double_open == 0)
			single_open = !single_open;
		else if (input[i] == 34 && single_open == 0)
			double_open = !double_open;
		i++;
	}
	if (single_open || double_open)
		return (0);
	return (1);
}

void	ft_echo(char *input)
{
	int	i;

	if (input[5] == '-' && input[6] == 'n'
		&& (input[7] == ' ' || input[7] == '\0'))
	{
		i = 8;
		while (input[i])
		{
			write(1, &input[i], 1);
			i++;
		}
		return ;
	}
	i = 5;
	while (input[i])
	{
		write(1, &input[i], 1);
		i++;
	}
	ft_printf("\n");
}

void	ft_remove_quotes(char *input)
{
	int		i;
	int		j;
	char	quote_type;

	i = 0;
	j = 0;
	quote_type = '\0';
	while (input[i])
	{
		if ((input[i] == 39 || input[i] == 34) && quote_type == '\0')
			quote_type = input[i];
		else if (input[i] == quote_type)
			quote_type = '\0';
		else
		{
			input[j] = input[i];
			j++;
		}
		i++;
	}
	if (quote_type != '\0')
		input[++j] = quote_type;
	input[j] = '\0';
}
int ft_pipe(char *input, char **envp, t_shell *shell)
{
	(void)shell;
	char	**cmds;
	int		pipefd[2];
    pid_t	pid;
	int		i;
	int		prev_fd;
	char	*path;
	
	i = 0;
	prev_fd = 0;
	cmds = ft_split(input, '|');
	if (!cmds)
		return (1);
	while (cmds[i])
	{
		if (cmds[i + 1] && pipe(pipefd) == -1)
		{
			free_cmd(cmds);
			return (1);
		}
		pid = fork();
		if (pid == -1)
		{
			free_cmd(cmds);
			return (1);
		}
		if (pid == 0)
		{
			if (cmds[i + 1])
				dup2(pipefd[1], STDOUT_FILENO);
			if (prev_fd != 0)
                dup2(prev_fd, STDIN_FILENO);
			close(pipefd[0]);
            close(pipefd[1]);
			path = find_command_path(cmds[i], envp);
			if (!path)
			{
				ft_printf("command not found: %s\n", input);
				free(path);
				free_cmd(cmds);
				exit(EXIT_FAILURE);
			}
			execve(path, cmds, envp);
			ft_printf("command not found: %s\n", input);
			free(path);
			free_cmd(cmds);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	if (prev_fd != 0)
        close(prev_fd);
	while (wait(NULL) > 0);
    free_cmd(cmds);
    return (1);
}

int	commands(char *input, char **envp, t_shell *shell)
{
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	if (*input == '\0')
		return (1);
	shell->args = ft_split(input, ' ');
	if (!ft_quotes(input))
		return (ft_printf("open quote\n"));
	if (ft_strchr(input, '|'))
		return (ft_pipe(input, envp, shell));
	ft_remove_quotes(input);
	if (ft_strncmp(input, "exit", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
		return (ft_exit(input));
	if (ft_strncmp(input, "echo", 4) == 0
		&& (input[4] == ' ' || input[4] == '\0'))
	{
		ft_echo(input);
		return (1);
	}
	ft_shell(input, envp, shell);
	return (1);
}
