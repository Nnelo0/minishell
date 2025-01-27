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

int	parse_redirection(char **args, char **commands, char **out_file, char **in_file, int *append)
{
	char	*tmp;
	int		i;

	i = -1;
	*append = 0;
	*in_file = NULL;
	*out_file = NULL;
	while (args[++i])
	{
		tmp = args[i];
		args[i] = ft_strtrim(args[i], " ");
		free(tmp);
		if (ft_strcmp(args[i], "<") == 0)
		{
			if (args[i + 1])
			{
				free(*in_file);
				*in_file = ft_strdup(args[++i]);
			}
			else
				return (free(*in_file), free_args(args), free(*commands), 1);
		}
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
		{
			if (ft_strcmp(args[i], ">>") == 0)
				*append = 1;
			else
				*append = 0;
			if (args[i + 1])
			{
				free(*out_file);
				*out_file = ft_strdup(args[i++ + 1]);
			}
			else
				return (free(*in_file), free_args(args), free(*commands), 1);
		}
		else if (!*commands)
			*commands = ft_strdup(args[i]);
		else
			parse_commands(commands, tmp, args[i]);
	}
	return (1);
}

int	ft_redirection(t_shell *shell)
{
	int		append;
	char	*in_file;
	char	*out_file;

	shell->fd_out = -1;
	shell->fd_in = -1;
	in_file = NULL;
	out_file = NULL;
	parse_redirection(shell->ipt_rdct, &shell->cmd, &out_file, &in_file, &append);
	if (!shell->cmd)
		return (1);
	if (in_file)
	{
		shell->fd_in = open(in_file, O_RDONLY);
		if (shell->fd_in == -1)
		return (perror(in_file), free(shell->cmd), 1);
	}
	if (out_file)
	{
		if (append)
			shell->fd_out = open(out_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			shell->fd_out = open(out_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (shell->fd_out == -1)
			return (perror(out_file), free(shell->cmd), 1);
	}
	which_commands(shell->cmd, shell->envp1, shell);
	if (shell->fd_in != -1)
		close(shell->fd_in);
	if (shell->fd_out != -1)
		close(shell->fd_out);
	return (wait(NULL), free(in_file), free(out_file), free(shell->cmd), 1);
}
