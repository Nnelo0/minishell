/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nnelo <nnelo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:53:43 by nnelo             #+#    #+#             */
/*   Updated: 2025/01/08 15:57:26 by nnelo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft/libft.h"
# include "libft/printf/ft_printf.h"
# include <sys/wait.h>
# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include "pipex.h"
# include <signal.h>

typedef struct s_shell
{
	sig_atomic_t	signal_status;
	char			**args;
}	t_shell;

void	handle_prompt(t_shell *shell, char **envp);
int		keypress(char *input, t_shell *shell);
void	handle_sigint(int sig);
int		ft_exit(char *input);
int		commands(char *input, char **envp, t_shell *shell);
void	ft_shell(char *input, char **envp, t_shell *shell);

#endif