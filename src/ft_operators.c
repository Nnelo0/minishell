/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_operators.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 15:34:36 by ebroudic          #+#    #+#             */
/*   Updated: 2025/01/13 17:33:48 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	ft_input_redirection(char *input, t_shell *shell)
// {
// 	int		fd_files;
// 	char	**args;
// 	char	*path;
// 	pid_t	pid;
// 	char	**cmd_args;
// 	char	*tmp;

// 	path = NULL;
// 	cmd_args = NULL;
// 	args = ft_split(input, '<');
// 	tmp = args[0];
// 	args[0] = ft_strtrim(args[0], " ");
// 	free(tmp);
// 	tmp = args[1];
// 	args[1] = ft_strtrim(args[1], " ");
// 	free(tmp);
// 	fd_files = open(args[1], O_RDONLY);
// 	if (fd_files == -1)
// 	{
// 		free_args(args);
// 		perror("files");
// 		return (1);
// 	}
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork failed");
// 		free_args(args);
// 		exit(EXIT_FAILURE);
// 	}
// 	if (pid == 0)
// 	{
// 		if (dup2(fd_files, STDIN_FILENO) == -1)
// 		{
// 			perror("dup2 failed");
// 			close(fd_files);
// 			free_args(args);
// 			exit(EXIT_FAILURE);
// 		}
// 		close(fd_files);
// 		cmd_args = ft_split(args[0], ' ');
// 		path = find_command_path(cmd_args[0], shell->envp1);
// 		if (!path)
// 		{
// 			ft_printf("command not found: %s\n", args[0]);
// 			free_args(cmd_args);
// 			exit(EXIT_FAILURE);
// 		}
// 		execve(path, cmd_args, shell->envp1);
// 		ft_printf("command not found: %s\n", args[0]);
// 		free(path);
// 		free_args(cmd_args);
// 		exit(EXIT_FAILURE);
// 	}
// 	close(fd_files);
// 	wait(NULL);
// 	free(path);
// 	free_args(args);
// 	free_args(cmd_args);
// 	return (1);
// }

int	ft_input_redirection(char *input, t_shell *shell)
{
	int		fd_temp, fd_file;
	char	**args;
	char	*path;
	pid_t	pid;
	char	**cmd_args;
	int		i;
	char	buffer[1024];
	ssize_t	bytes_read;

	// Diviser l'entrée par '<' et nettoyer les espaces
	args = ft_split(input, '<');
	if (!args || !args[0])
		return (write(2, "Invalid input\n", 15), free_args(args), 1);
	i = 0;
	while (args[i])
	{
		char *tmp = args[i];
		args[i] = ft_strtrim(args[i], " ");
		free(tmp);
		i++;
	}

	// Vérifier qu'il y a au moins une commande et un fichier
	if (i < 2)
		return (write(2, "No file specified\n", 18), free_args(args), 1);

	// Créer un fichier temporaire pour stocker la concaténation
	fd_temp = open(".temp_input_redirection", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_temp == -1)
		return (perror("Temporary file creation failed"), free_args(args), 1);

	// Lire chaque fichier spécifié après '<' et écrire dans le fichier temporaire
	for (int j = 1; args[j]; j++)
	{
		fd_file = open(args[j], O_RDONLY);
		if (fd_file == -1)
		{
			perror(args[j]);
			close(fd_temp);
			unlink(".temp_input_redirection");
			return (free_args(args), 1);
		}
		while ((bytes_read = read(fd_file, buffer, sizeof(buffer))) > 0)
			write(fd_temp, buffer, bytes_read);
		if (bytes_read == -1)
			perror("Error reading file");
		close(fd_file);
	}
	close(fd_temp);

	// Créer un processus pour exécuter la commande avec l'entrée temporaire
	pid = fork();
	if (pid == -1)
	{
		perror("fork failed");
		unlink(".temp_input_redirection");
		return (free_args(args), 1);
	}
	if (pid == 0) // Processus enfant
	{
		fd_temp = open(".temp_input_redirection", O_RDONLY);
		if (fd_temp == -1)
		{
			perror("Temporary file open failed");
			exit(1);
		}
		if (dup2(fd_temp, STDIN_FILENO) == -1)
		{
			perror("dup2 failed");
			close(fd_temp);
			exit(1);
		}
		close(fd_temp);

		// Exécuter la commande
		cmd_args = ft_split(args[0], ' ');
		path = find_command_path(cmd_args[0], shell->envp1);
		if (!path)
		{
			write(2, "command not found: ", 19);
			write(2, args[0], ft_strlen(args[0]));
			write(2, "\n", 1);
			free_args(cmd_args);
			exit(127); // Code standard pour "command not found"
		}
		execve(path, cmd_args, shell->envp1);
		perror("execve failed");
		free(path);
		free_args(cmd_args);
		exit(1); // Code générique pour échec
	}

	// Attendre la fin du processus enfant
	wait(NULL);

	// Nettoyer et supprimer le fichier temporaire
	unlink(".temp_input_redirection");
	free_args(args);
	return (1);
}



