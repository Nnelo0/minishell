/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 09:47:40 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/03 09:47:42 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_exit(char **input, t_shell *shell)
{
	ft_putstr_fd("exit\n", 1);
	if (input[1])
		ft_remove_quotes(input[1]);
	if (input[1])
	{
		if (ft_isdigit_s(input[1]))
		{
			shell->status = ft_atoi(input[1]);
			if (input[2])
				return (shell->status = 1,
					ft_putstr_fd(" too many arguments\n", 2), shell->status);
		}
		if (!ft_isdigit_s(input[1]) || ft_strlen(input[1]) >= 20)
		{
			shell->status = 2;
			ft_putstr_fd(input[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
		}
	}
	return (free_all(shell), rl_clear_history(),
		exit(shell->status), shell->status);
}

int	ft_exe(char **args, char **envp, t_shell *shell)
{
	pid_t		pid;
	int			status;
	struct stat	path_stat;

	if (!args || !args[0])
		return (ft_putstr_fd("command not found\n", 2), 1);
	if (stat(args[0], &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
			return (ft_putstr_fd(args[0], 2),
				ft_putstr_fd(": Is a directory\n", 2), 126);
		if (S_ISREG(path_stat.st_mode) && access(args[0], X_OK) == -1)
			return (ft_putstr_fd(args[0], 2),
				ft_putstr_fd(": Permission denied\n", 2), 126);
	}
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	envp = env_in_stars(shell);
	if (pid == 0)
		ft_execute(args, envp, shell);
	return (waitpid(pid, &status, 0), free_args(envp), (status >> 8) & 0xFF);
}

int	ft_cd(char **args)
{
	char	*target;

	if (!args[1])
	{
		target = getenv("HOME");
		if (!target)
			return (ft_putstr_fd("HOME not set\n", 2), 1);
	}
	else if (args[2])
		return (ft_putstr_fd(" too many arguments\n", 2), 1);
	else
		target = args[1];
	if (chdir(target) == -1)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	ft_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_printf("%s\n", pwd);
	free(pwd);
	return (0);
}

int	ft_env(t_shell *shell)
{
	t_env	*temp;

	if (!shell || !shell->env_list)
		return (1);
	temp = shell->env_list;
	while (temp)
	{
		printf("%s\n", temp->value);
		temp = temp->next;
	}
	free(temp);
	return (0);
}
