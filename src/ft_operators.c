/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 20:50:58 by nnelo             #+#    #+#             */
/*   Updated: 2025/02/17 20:04:15 by nnelo            ###   ########.fr       */
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

void	parse_commands(char **commands, char *tmp, char *args)
{
	char	*tmp_cmd;

	tmp = ft_strjoin(*commands, " ");
	tmp_cmd = ft_strjoin(tmp, args);
	free(tmp);
	free(*commands);
	*commands = tmp_cmd;
}

int	parse_redirection(t_shell *shell, int *out_count, int *append, int i)
{
	char	*tmp;

	*append = 0;
	shell->in_file = NULL;
	*out_count = 0;
	while (shell->input[++i])
	{
		tmp = shell->input[i];
		shell->input[i] = ft_strtrim(shell->input[i], " ");
		free(tmp);
		if (parse_heredoc(shell, i))
			return (1);
		else if (parse_in(shell, i) || parse_out(shell, i, out_count, append))
			return (1);
		else if (ft_strcmp(shell->input[i], "<") == 0 || ft_strcmp
			(shell->input[i], ">") == 0 || ft_strcmp(shell->input[i], ">>") == 0
			|| ft_strcmp(shell->input[i], "<<") == 0)
			i++;
		else if (!shell->cmd)
			shell->cmd = ft_strdup(shell->input[i]);
		else
			parse_commands(&(shell->cmd), tmp, shell->input[i]);
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
				free(shell->in_file), free(shell->out_file), 1);
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

int	ft_redirection(t_shell *shell)
{
	int		append;
	int		out_count;

	shell->in_file = NULL;
	shell->fd_in = -1;
	shell->fd_out = -1;
	shell->out_file = malloc(sizeof(char *) * (ft_strlen_tab(shell->input) + 1));
	if (!shell->out_file)
		return (127);
	out_count = 0;
	parse_redirection(shell, &out_count, &append, -1);
	if (!shell->cmd)
		return (127);
	if (open_files(shell, out_count, append))
		return (127);
	shell->cmds = malloc(sizeof(char *) * ft_strlen_tab (shell->input) + 1);
	if (!shell->cmds)
		return (127);
	shell->cmds[0] = ft_strdup(shell->cmd);
	for (int i = 0; shell->cmds[i]; i++)
		printf("{%s}\n", shell->cmds[i]);
	//which_commands(shell->cmd, shell->envp1, shell);
	if (shell->fd_in != -1)
		close(shell->fd_in);
	if (shell->fd_out != -1)
		close(shell->fd_out);
	return (wait(NULL), free(shell->in_file), free(shell->cmd),
		free_args(shell->out_file), 0);
}

//rentrer dans ft_redirection que si y'a un <> sinon segfault je crois, la partie shell->cmds est un test 
//pas sur que ca marche mais si ca marche tant mieux j'ai rien changer d'autres, si ca marche faudra changer les pipes
//car il ne sont pas dans which commands ou alors changer l'input ici et faire en sorte que ca rentre plus dans wich_commands ici comme ca ont fais les redirections en premier et apres 
//ca fait la suite du programme sans mettre les pipes dans wich_commands ca sera plus simple je pense 