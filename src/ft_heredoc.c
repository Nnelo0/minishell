/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:31:41 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/18 16:31:31 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	create_tmp_file(void)
{
	char	*filename;
	int		fd;

	filename = ".tmp_heredoc";
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		perror("open failed");
	return (fd);
}

void	read_heredoc(t_shell *shell, char *delimiter)
{
	char	*line;
	int		fd;

	fd = create_tmp_file();
	if (fd == -1)
		return ;
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd);
		ft_putstr_fd("\n", fd);
		free(line);
	}
	close(fd);
	shell->fd_in = open(".tmp_heredoc", O_RDONLY);
	if (shell->fd_in == -1)
		perror("open failed");
	unlink(".tmp_heredoc");
}

int	parse_heredoc(char **input, t_shell *shell, int i)
{
	if (ft_strcmp(input[i], "<<") == 0)
	{
		if (!input[i + 1])
			return (1);
		read_heredoc(shell, input[++i]);
	}
	return (0);
}
