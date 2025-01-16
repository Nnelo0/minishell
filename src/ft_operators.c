/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:34:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/16 15:04:35 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

char	**ft_split_chevrons(char *input, int i, int j)
{
	char	**res;
	char	*start;

	if (!input)
		return (NULL);
	res = malloc(sizeof (char *) * (ft_strlen(input) + 1));
	if (!res)
		return (NULL);
	start = input;
	while (input[++i])
	{
		if (input[i] == '<' || input[i] == '>' || input[i] == ' ')
		{
			if (start != input + i)
				res[j++] = ft_substr(start, 0, input + i - start);
			if (input[i] != ' ')
				res[j++] = ft_substr(input + i, 0, 1);
			start = input + i + 1;
		}
	}
	if (start != input + i)
		res[j++] = ft_substr(start, 0, input + i - start);
	return (res[j] = NULL, res);
}

int	parse_redirection(char **args, char **commands, char **files, int *fcount)
{
	char	*tmp;
	char	*tmp_cmd;
	int		i;

	i = -1;
	while (args[++i])
	{
		tmp = args[i];
		args[i] = ft_strtrim(args[i], " ");
		free(tmp);
		if (ft_strcmp(args[i], "<") == 0)
		{
			if (args[i + 1])
				files[(*fcount)++] = ft_strdup(args[i++ + 1]);
			else
				return (files[(*fcount)] = NULL, free_args(args),
					free_args(files), free(*commands), 1);
		}
		else if (!*commands)
			*commands = ft_strdup(args[i]);
		else
		{
			tmp = ft_strjoin(*commands, " ");
			tmp_cmd = ft_strjoin(tmp, args[i]);
			free(tmp);
			free(*commands);
			*commands = tmp_cmd;
		}
	}
	files[(*fcount)] = NULL;
	return (1);
}

int	is_valid_chevrons(char *input)
{
	int	i;
	int	expect_file;

	i = 0;
	expect_file = 0;
	while (input[i])
	{
		if (input[i] == '<')
		{
			if (input[i + 1] == '<' || input[i + 1] == '\0')
				return (0);
			expect_file = 1;
		}
		else if (input[i] != ' ' && expect_file)
			expect_file = 0;
		i++;
	}
	return (expect_file == 0);
}

void	ft_execute_cmd(int fd_files, char **args, char **files, t_shell *shell)
{
	if (dup2(fd_files, STDIN_FILENO) == -1)
	{
		perror("dup2 failed");
		close(fd_files);
		free(shell->cmd);
		free_args(args);
		free_args(files);
		exit(1);
	}
	close(fd_files);
	which_commands(shell->cmd, shell->envp1, shell);
	free(shell->cmd);
	free_args(args);
	free_args(files);
	free_args(shell->args);
	exit (0);
}

int	ft_input_redirection(char *input, t_shell *shell)
{
	char	**args;
	char	**files;
	pid_t	pid;
	int		fd_files;
	int		fcount;

	if (!is_valid_chevrons(input))
		return (ft_printf("invalid '<'\n"));
	fd_files = 0;
	fcount = 0;
	shell->cmd = NULL;
	args = ft_split_chevrons(input, -1, 0);
	if (!args)
		return (write(2, "ft_split failed\n", 23), 1);
	files = malloc(sizeof (char *) * (ft_strlen_tab(args) + 1));
	if (!files)
		return (free_args(args), write(2, "allocation failed\n", 18), 1);
	parse_redirection(args, &shell->cmd, files, &fcount);
	if (!shell->cmd)
		return (free_args(args), free_args(files), 1);
	fd_files = open(files[fcount - 1], O_RDONLY);
	if (fd_files == -1)
		return (perror(files[fcount - 1]), free(shell->cmd), free_args(args), free_args(files), 1);
	pid = fork();
	if (pid == -1)
		return (free(shell->cmd), free_args(args), free_args(files), close(fd_files), perror("fork failed"), 1);
	if (pid == 0)
		ft_execute_cmd(fd_files, args, files, shell);
	close(fd_files);
	while (wait(NULL) > 0)
		;
	return (free(shell->cmd), free_args(args), free_args(files), 1);
}
