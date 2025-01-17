/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_output.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 09:53:07 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/17 12:50:37 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_out_redirec(char **args, char **commands, char **files, int *fcount)
{
	char	*tmp;
	int		i;

	i = -1;
	*fcount = 0;
	while (args[++i])
	{
		tmp = args[i];
		args[i] = ft_strtrim(args[i], " ");
		free(tmp);
		if (ft_strcmp(args[i], ">") == 0)
		{
			if (args[i + 1])
				files[(*fcount)++] = ft_strdup(args[i++ + 1]);
			else
				return (files[(*fcount)] = NULL,
					free_args(files), free(*commands), 1);
		}
		else if (!*commands)
			*commands = ft_strdup(args[i]);
		else
			parse_commands(commands, tmp, args[i]);
	}
	files[(*fcount)] = NULL;
	return (1);
}

void	ft_execute_cmd_out(int fd_files, char **files, t_shell *shell)
{
	if (dup2(fd_files, STDOUT_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd_files);
		free(shell->cmd);
		free_args(files);
		exit(1);
	}
	close(fd_files);
	which_commands(shell->cmd, shell->envp1, shell);
	free(shell->cmd);
	free_args(files);
	free_args(shell->args);
	exit (0);
}

int	ft_output_redirection(char **args, t_shell *shell, int i)
{
	char	**files;
	pid_t	pid;
	int		fd_files;
	int		fcount;

	files = malloc(sizeof (char *) * (ft_strlen_tab(args) + 1));
	if (!files)
		return (write(2, "allocation failed\n", 18), 1);
	parse_out_redirec(args, &shell->cmd, files, &fcount);
	if (!shell->cmd)
		return (free_args(files), 1);
	while (--fcount >= 0 && ++i + 1)
		fd_files = open(files[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_files == -1)
		return (perror(files[fcount - 1]), free(shell->cmd),
			free_args(files), 1);
	pid = fork();
	if (pid == -1)
		return (free(shell->cmd), free_args(files),
			close(fd_files), perror("fork failed"), 1);
	if (pid == 0)
		ft_execute_cmd_out(fd_files, files, shell);
	return (close(fd_files), wait(NULL), free(shell->cmd),
		free_args(files), 1);
}
