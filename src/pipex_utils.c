/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 09:18:33 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/06 10:19:29 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	open_files(int files[2], char **argv, int fd[2])
{
	files[0] = open(argv[1], O_RDONLY);
	files[1] = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (files[0] == -1 && files[1] == -1)
	{
		close_all(files, fd);
		exit(ft_printf("%s: No such file or permission denied\n\
%s: permission denied\n", argv[1], argv[4]));
	}
	if (files[0] == -1)
	{
		close_all(files, fd);
		exit(ft_printf("pipex: No such file or permission denied: %s\n", \
		argv[1]));
	}
	if (files[1] == -1)
	{
		close_all(files, fd);
		exit(ft_printf("%s: permission denied\n", argv[4]));
	}
}

void	free_paths(char **paths)
{
	int	i;

	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
}

char	*find_command_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_paths(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_paths(paths);
	return (0);
}

void	check_arg(char **argv)
{
	if ((!argv[2] || argv[2][0] == '\0') && (!argv[3] || argv[3][0] == '\0'))
		exit(ft_printf("pipex: cmd1 is empty\npipex: cmd2 is empty\n"));
	if (!argv[2] || argv[2][0] == '\0')
		exit(ft_printf("pipex: cmd1 is empty\n"));
	if (!argv[3] || argv[3][0] == '\0')
		exit(ft_printf("pipex: cmd2 is empty\n"));
}

void	check_command(char **argv, char **envp)
{
	char	*path;
	char	*path2;
	char	**cmd2;
	char	**cmd1;

	cmd1 = ft_split(argv[2], ' ');
	path = find_command_path(*cmd1, envp);
	cmd2 = ft_split(argv[3], ' ');
	path2 = find_command_path(*cmd2, envp);
	if (!path && !path2)
	{
		free_cmd(cmd1);
		free_cmd(cmd2);
		exit(write(2, "pipex: command not found for cmd1 and cmd2\n", 43));
	}
	if (!path)
	{
		free_cmd(cmd1);
		exit(write(2, "pipex: command not found for cmd1\n", 34));
	}
	if (!path2)
	{
		free_cmd(cmd2);
		exit(write(2, "pipex: command not found for cmd2\n", 34));
	}
}
