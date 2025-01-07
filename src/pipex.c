/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:55:46 by nnelo             #+#    #+#             */
/*   Updated: 2025/01/07 12:48:45 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_cmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i] != NULL)
	{
		free(cmd[i]);
		i++;
	}
	free(cmd);
}

void	close_all(int files[2], int fd[2])
{
	close(fd[0]);
	close(fd[1]);
	close(files[0]);
	close(files[1]);
}

static void	write_in_pipe(int files[2], int fd[2], char **argv, char **envp)
{
	char	**cmd1;
	char	*path;

	dup2(files[0], STDIN_FILENO);
	dup2(fd[1], STDOUT_FILENO);
	close_all(files, fd);
	cmd1 = ft_split(argv[2], ' ');
	if (!cmd1)
		exit(1);
	path = find_command_path(*cmd1, envp);
	if (!path)
	{
		write(2, "pipex: command not found for cmd1\n", 34);
		free_cmd(cmd1);
		exit(1);
	}
	execve(path, cmd1, envp);
	write(2, "pipex: command not found for cmd1\n", 34);
	free_cmd(cmd1);
	free(path);
	exit(1);
}

static void	write_in_file(int files[2], int fd[2], char **argv, char **envp)
{
	char	**cmd2;
	char	*path;

	dup2(fd[0], STDIN_FILENO);
	dup2(files[1], STDOUT_FILENO);
	close_all(files, fd);
	cmd2 = ft_split(argv[3], ' ');
	if (!cmd2)
		exit(1);
	path = find_command_path(*cmd2, envp);
	if (!path)
	{
		write(2, "pipex: command not found for cmd2\n", 34);
		free_cmd(cmd2);
		exit(1);
	}
	execve(path, cmd2, envp);
	write(2, "pipex: command not found for cmd2\n", 34);
	free_cmd(cmd2);
	free(path);
	exit(1);
}

int	pipex(int argc, char **argv, char **envp)
{
	int		files[2];
	int		fd[2];
	pid_t	pid;

	if (argc != 5)
		exit(ft_printf("Usage: ./pipex infile cmd1 cmd2 outfile\n"));
	check_arg(argv);
	check_command(argv, envp);
	if (pipe(fd) == -1)
		exit(ft_printf("pipe error\n"));
	open_files(files, argv, fd);
	pid = fork();
	if (pid == -1)
		exit(ft_printf("fork error\n"));
	if (pid == 0)
		write_in_pipe(files, fd, argv, envp);
	if (fork() == 0)
		write_in_file(files, fd, argv, envp);
	close_all(files, fd);
	wait(NULL);
	wait(NULL);
	return (0);
}
