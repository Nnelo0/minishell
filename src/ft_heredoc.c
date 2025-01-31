/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 13:31:41 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/31 15:28:07 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	signal_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(1);
}

static int create_tmp_file(void)
{
    char *filename = ".tmp_heredoc";
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
        perror("open failed");
    return fd;
}

void read_heredoc(t_shell *shell, char *delimiter)
{
    char *line;
    int fd;

    signal(SIGINT, signal_heredoc);
    fd = create_tmp_file();
    if (fd == -1)
        return;
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
		ft_putstr_fd(line, fd);
        ft_putstr_fd("\n", fd);
        free(line);
    }
    close(fd);
    shell->fd_in = open(".tmp_heredoc", O_RDONLY);
    if (shell->fd_in == -1)
        perror("open failed");
}
