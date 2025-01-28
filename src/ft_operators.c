/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:34:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/17 12:49:14 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_has_redirection(char **ipt_rdct, const char *redir)
{
	int	i;

	i = 0;
	if (!ipt_rdct || !redir)
		return (0);
	while (ipt_rdct[i])
	{
		if (ft_strcmp(ipt_rdct[i], redir) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	ft_strlen_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return (0);
	while (tab[i] != NULL)
		i++;
	return (i);
}

void	parse_commands(char **commands, char *tmp, char *args)
{
	char	*tmp_cmd;

	tmp = ft_strjoin(*commands, " ");
	tmp_cmd = ft_strjoin(tmp, args);
	free(tmp);
	free(*commands);
	*commands = tmp_cmd;
}

int	parse_redirection(t_shell *shell, int *out_count, int *append)
{
	char	*tmp;
	int		i;

	i = -1;
	*append = 0;
	shell->in_file = NULL;
	*out_count = 0;
	while (shell->ipt[++i])
	{
		tmp = shell->ipt[i];
		shell->ipt[i] = ft_strtrim(shell->ipt[i], " ");
		free(tmp);
		if (ft_strcmp(shell->ipt[i], "<") == 0)
		{
			if (shell->ipt[i + 1])
			{
				free(shell->in_file);
				shell->in_file = ft_strdup(shell->ipt[++i]);
			}
			else
				return (free(shell->in_file), free_args(shell->ipt),
					free(shell->cmd), 1);
		}
		else if (ft_strcmp(shell->ipt[i], ">") == 0
			|| ft_strcmp(shell->ipt[i], ">>") == 0)
		{
			if (ft_strcmp(shell->ipt[i], ">>") == 0)
				*append = 1;
			else
				*append = 0;
			if (shell->ipt[i + 1])
				shell->out_file[(*out_count)++] = ft_strdup(shell->ipt[i++ + 1]);
			else
				return (shell->out_file[(*out_count)] = NULL,
					free(shell->in_file), free_args(shell->ipt),
					free(shell->cmd), 1);
		}
		else if (!shell->cmd)
			shell->cmd = ft_strdup(shell->ipt[i]);
		else
			parse_commands(&(shell->cmd), tmp, shell->ipt[i]);
	}
	shell->out_file[(*out_count)] = NULL;
	return (1);
}

int	ft_redirection(t_shell *shell)
{
	int		append;
	int		i;
	int		out_count;

	shell->in_file = NULL;
	shell->fd_in = -1;
	shell->fd_out = -1;
	shell->out_file = malloc(sizeof(char *) * (ft_strlen_tab(shell->ipt) + 1));
	if (!shell->out_file)
		return (1);
	out_count = 0;
	parse_redirection(shell, &out_count, &append);
	if (!shell->cmd)
		return (1);
	i = -1;
	if (shell->in_file)
	{
		shell->fd_in = open(shell->in_file, O_RDONLY);
		if (shell->fd_in == -1)
		return (perror(shell->in_file), free(shell->cmd), free(shell->in_file), 1);
	}
	while (--out_count >= 0 && ++i + 1)
	{
		if (append)
			shell->fd_out = open(shell->out_file[i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			shell->fd_out = open(shell->out_file[i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_count != 0)
			close(shell->fd_out);
	}
	which_commands(shell->cmd, shell->envp1, shell);
	if (shell->fd_in != -1)
		close(shell->fd_in);
	if (shell->fd_out != -1)
		close(shell->fd_out);
	return (wait(NULL), free(shell->in_file), free(shell->cmd), free_args(shell->out_file), 1);
}
