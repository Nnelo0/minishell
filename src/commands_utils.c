/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 14:12:58 by ebroudic          #+#    #+#             */
/*   Updated: 2025/02/26 15:18:16 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_separator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

char	*ft_add_space(char *input, int i)
{
	int		len;
	char	*result;
	int		in_quotes;

	if (!input)
		return (NULL);
	result = malloc(ft_strlen(input) * 2 + 1);
	if (!result)
		return (free(input), NULL);
	len = 0;
	in_quotes = 0;
	while (input[++i])
	{
		if (input[i] == '"' || input[i] == '\'')
			in_quotes = !in_quotes;
		if (!in_quotes && i > 0 && is_separator(input[i])
			&& input[i - 1] != ' ' && !is_separator(input[i - 1]))
			result[len++] = ' ';
		result[len++] = input[i];
		if (!in_quotes && is_separator(input[i])
			&& input[i + 1] != ' ' && !is_separator(input[i + 1]))
			result[len++] = ' ';
	}
	return (result[len] = '\0', result);
}

void	setup_redirection(t_shell *shell)
{
	shell->save_in = dup(STDIN_FILENO);
	if (shell->save_in == -1)
		perror("dup IN");
	shell->save_out = dup(STDOUT_FILENO);
	if (shell->save_out == -1)
		perror("dup OUT");
	if (shell->fd_in != -1)
	{
		if (dup2(shell->fd_in, STDIN_FILENO) == -1)
			perror("dup2 IN");
		close(shell->fd_in);
		shell->fd_in = -1;
	}
	if (shell->fd_out != -1)
	{
		if (dup2(shell->fd_out, STDOUT_FILENO) == -1)
			perror("dup2 OUT");
		close(shell->fd_out);
		shell->fd_out = -1;
	}
}
