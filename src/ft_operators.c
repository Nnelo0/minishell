/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:34:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/14 09:16:58 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_input_redirection(char *input, t_shell *shell)
{
	int		fd_files;
	char	**args;
	char	*path;
	pid_t	pid;
	char	**cmd_args;
	char	*tmp;
	int		i;

	i = 0;
	path = NULL;
	cmd_args = NULL;
	args = ft_split(input, '<');
	while (args[i])
	{
		tmp = args[i];
		args[i] = ft_strtrim(args[i], " ");
		free(tmp);
		i++;
	}
	fd_files = open(args[i - 1], O_RDONLY);
	if (fd_files == -1)
	{
		perror(args[i - 1]);
		free_args(args);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		free_args(args);
		close(fd_files);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (dup2(fd_files, STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			close(fd_files);
			free_args(args);
			exit(EXIT_FAILURE);
		}
		close(fd_files);
		cmd_args = ft_split(args[0], ' ');
		path = find_command_path(cmd_args[0], shell->envp1);
		if (!path)
		{
			ft_printf("command not found: %s\n", args[0]);
			free_args(cmd_args);
			exit(EXIT_FAILURE);
		}
		execve(path, cmd_args, shell->envp1);
		ft_printf("command not found: %s\n", args[0]);
		free(path);
		free_args(cmd_args);
		exit(EXIT_FAILURE);
	}
	close(fd_files);
	wait(NULL);
	free(path);
	free_args(args);
	free_args(cmd_args);
	return (1);
}
