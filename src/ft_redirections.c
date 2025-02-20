/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_redirections.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:50:58 by nnelo             #+#    #+#             */
/*   Updated: 2025/02/20 12:24:16 by ebroudic         ###   ########.fr       */
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

int	parse_redirection(t_shell *shell, int *out_count, int *append, int i)
{
	char	*tmp;

	*append = 0;
	shell->in_file = NULL;
	*out_count = 0;
	while (shell->copy[++i])
	{
		tmp = shell->copy[i];
		shell->copy[i] = ft_strtrim(shell->copy[i], " ");
		free(tmp);
		if (parse_heredoc(shell, i))
			return (1);
		else if (parse_in(shell, i) || parse_out(shell, i, out_count, append))
			return (1);
		else if (ft_strcmp(shell->copy[i], "<") == 0 || ft_strcmp
			(shell->copy[i], ">") == 0 || ft_strcmp(shell->copy[i], ">>") == 0
			|| ft_strcmp(shell->copy[i], "<<") == 0)
			i++;
		else if (!shell->cmd)
			shell->cmd = ft_strdup(shell->copy[i]);
		else
			parse_commands(&(shell->cmd), tmp, shell->copy[i]);
	}
	return (shell->out_file[(*out_count)] = NULL, 0);
}

int	open_files(t_shell *shell, int out_count, int append)
{
	int	i;

	i = -1;
	if (shell->in_file)
	{
		shell->fd_in = open(shell->in_file, O_RDONLY);
		if (shell->fd_in == -1)
			return (perror(shell->in_file), free(shell->cmd),
				free(shell->in_file), free(shell->out_file), shell->status = 1);
	}
	while (--out_count >= 0 && ++i + 1)
	{
		if (append)
			shell->fd_out = open(shell->out_file[i],
					O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			shell->fd_out = open(shell->out_file[i],
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (out_count != 0)
			close(shell->fd_out);
	}
	return (0);
}

char	**ft_parse(char **input, int append, t_shell *shell)
{
	int		out_count;
	char	**new_input;

	shell->in_file = NULL;
	shell->fd_in = -1;
	shell->fd_out = -1;
	shell->out_file = malloc(sizeof(char *) * (ft_strlen_tab(input) + 1));
	if (!shell->out_file)
		return (NULL);
	out_count = 0;
	shell->copy = copy_string(input);
	if (!shell->copy)
		return (NULL);
	parse_redirection(shell, &out_count, &append, -1);
	if (open_files(shell, out_count, append))
		return (NULL);
	if (!shell->cmd)
		return (free(shell->in_file), free(shell->cmd),
			free_args(shell->out_file), NULL);
	new_input = ft_split(shell->cmd, ' ');
	if (!new_input)
		return (NULL);
	return (free(shell->in_file), free(shell->cmd),
		free_args(shell->out_file), new_input);
}

char	**ft_redirection(char **input, t_shell *shell)
{
	int		i;
	char	**new_input;

	i = 0;
	while (input[i])
	{
		if (ft_strchr(input[i], '<') || ft_strchr(input[i], '>'))
		{
			new_input = ft_parse(input, 0, shell);
			free_args(shell->copy);
			return (new_input);
		}
		i++;
	}
	return (input);
}
