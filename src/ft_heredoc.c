/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:31:41 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/20 14:51:02 by ebroudic         ###   ########.fr       */
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

int	write_heredoc(char *line, char *delimiter, int fd)
{
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0
		&& line[ft_strlen(delimiter)] == '\n')
	{
		close(fd);
		free(line);
		return (1);
	}
	ft_putstr_fd(line, fd);
	free(line);
	return (0);
}

void	read_heredoc(t_shell *shell, char *delimiter)
{
	char	*line;

	shell->fd_in = create_tmp_file();
	if (shell->fd_in == -1)
		return ;
	g_status = 42;
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(0);
		if (!line || line[0] == '\0')
		{
			free(line);
			ft_putstr_fd("\nHeredoc: warning: end-of-file \
(CTRL+D) detected\n", 2);
			break ;
		}
		if (g_status == 43)
		{
			get_next_line(-1);
			free(line);
			shell->status = 130;
			break ;
		}
		if (write_heredoc(line, delimiter, shell->fd_in) == 1)
			break ;
	}
	close(shell->fd_in);
	unlink(".tmp_heredoc");
}

int	parse_heredoc(t_shell *shell, int i)
{
	if (ft_strcmp(shell->copy[i], "<<") == 0)
	{
		if (!shell->copy[i + 1])
			return (1);
		read_heredoc(shell, shell->copy[++i]);
	}
	return (0);
}
