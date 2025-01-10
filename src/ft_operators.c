/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 08:45:49 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/10 09:55:24 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_input_redirection(char *input, t_shell *shell)
{
	int		fd_files;
	char	**args;
	char	*path;
	pid_t	pid;
	char **cmd_args;

	path = NULL;
	cmd_args = NULL;
	args = ft_split(input, '<');
	args[0] = ft_strtrim(args[0], " ");
	args[1] = ft_strtrim(args[1], " ");
	fd_files = open(args[1], O_RDONLY);
	if (fd_files == -1){
		free_args(args);
		perror("files");
		return(1); }
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (dup2(fd_files, STDIN_FILENO) == -1) {
			perror("dup2 failed");
			close(fd_files);
			exit(EXIT_FAILURE);
        }
		close(fd_files);
		cmd_args = ft_split(args[0], ' ');
		free_args(args);
		path = find_command_path(cmd_args[0], shell->envp1);
		if (!path)
		{
			ft_printf("command not found: %s\n", args);
			free(path);
			free_args(cmd_args);
			exit(EXIT_FAILURE);
		}
		execve(path, cmd_args, shell->envp1);
		ft_printf("command not found: %s\n", args);
		free(path);
		free_args(cmd_args);
		exit(EXIT_FAILURE);
	}
	close(fd_files);
	wait(NULL);
	free(path);
	free_args(cmd_args);
	return (1);
}
