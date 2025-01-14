/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:34:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/14 14:10:55 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_strlen_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (0);
	while (array[i] != NULL)
		i++;
	return (i);
}

int	ft_input_redirection(char *input, t_shell *shell)
{
	char	**args;
	char	**files;
	char	*path;
	char	*tmp;
	char	*tmp_cmd;
	pid_t	pid;
	char	**cmd_args;
	int		i;
	int		fd_files;
	int		file_count;
	char	*commands;

	file_count = 0;
	i = 0;
	path = NULL;
	cmd_args = NULL;
	commands = NULL;
	args = ft_split(input, ' ');
	if (!args)
		return (write(2, "ft_split failed\n", 23), 1);
	files = malloc(sizeof (char *) * (ft_strlen_array(args) + 1));
	if (!files)
	{
		free_args(args);
		return (write(2, "memory allocation for files failed\n", 41), 1);
	}
	while (args[i])
	{
		tmp = args[i];
		args[i] = ft_strtrim(args[i], " ");
		free(tmp);
		if (ft_strcmp(args[i], "<") == 0)
		{
			if (args[i + 1])
			{
				files[file_count++] = ft_strdup(args[i + 1]);
				i++;
			}
			else
			{
				files[file_count] = NULL;
				write(2, "syntax error '<'\n", 17);
				free_args(args);
				free_args(files);
				return (1);
			}
		}
		else if (!commands)
			commands = ft_strdup(args[i]);
		else
		{
			tmp = ft_strjoin(commands, " ");
			tmp_cmd = ft_strjoin(tmp, args[i]);
			free(tmp);
			free(commands);
			commands = tmp_cmd;
		}
		i++;
	}
	files[file_count] = NULL;
	if (!commands)
	{
		write(2, "undefined command\n", 18);
		free_args(args);
		return (1);
	}
	fd_files = open(files[file_count - 1], O_RDONLY);
	if (fd_files == -1)
	{
		perror(files[file_count - 1]);
		free(commands);
		free_args(args);
		free_args(files);
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		free(commands);
		free_args(args);
		free_args(files);
		close(fd_files);
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (dup2(fd_files, STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			close(fd_files);
			free(commands);
			free_args(args);
			free_args(files);
			exit(EXIT_FAILURE);
		}
		close(fd_files);
		cmd_args = ft_split(commands, ' ');
		path = find_command_path(cmd_args[0], shell->envp1);
		if (!path)
		{
			ft_printf("command not found: %s\n", args[0]);
			free(commands);
			free_args(cmd_args);
			free_args(files);
			exit(EXIT_FAILURE);
		}
		execve(path, cmd_args, shell->envp1);
		ft_printf("command not found: %s\n", args[0]);
		free(path);
		free(commands);
		free_args(cmd_args);
		free_args(files);
		exit(EXIT_FAILURE);
	}
	close(fd_files);
	wait(NULL);
	free(commands);
	free_args(args);
	free_args(files);
	return (1);
}
