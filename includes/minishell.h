/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cle-berr <cle-berr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 17:53:43 by nnelo             #+#    #+#             */
/*   Updated: 2025/01/06 15:03:32 by cle-berr         ###   ########.fr       */
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
# include <signal.h>

typedef struct s_shell
{
	volatile	sig_atomic_t	signal_status;
}	t_shell;
void	handle_prompt(t_shell *shell);
int		keypress(char *input, t_shell *shell);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

#endif