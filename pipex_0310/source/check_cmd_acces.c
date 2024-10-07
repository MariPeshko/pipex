/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_cmd_acces.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 11:23:03 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/07 16:18:22 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

/**
 * @brief This function frees the memory allocated for both the command 
 * and its path. Helps to clean up dynamically allocated memory after using 
 * commands and paths to prevent memory leaks.
 * 
 * @param cmd_plus_args: An array of strings that holds the command 
 * and its arguments.
 * @param cmd_path: A string that stores the full path of the command.
 */
void	free_cmd_and_path(char **cmd_plus_args, char *cmd_path)
{
	ft_freestr(cmd_plus_args);
	free(cmd_path);
}

/**
 * @brief This function checks if a given command can be accessed and 
 * executed by the system.
 * 
 * Called in functions "multi_pipe" and "here_doc".
 * 
 * The function returns -6 when either:
 * - The argument (command) is empty or NULL.
 * - The command is not found in the system’s path (i.e., get_path
 * cannot locate it).
 */
int	acces_cmd(char *argument, char *env[])
{
	char	**cmd_plus_args;
	char	*cmd_path;

	if (!*argument || !argument)
		return (-6);
	cmd_plus_args = ft_split(argument, ' ');
	if (!cmd_plus_args)
		perror_and_exit("malloc", NULL);
	if (access(cmd_plus_args[0], F_OK | X_OK) == 0)
	{
		ft_freestr(cmd_plus_args);
		return (0);
	}
	cmd_path = get_path(cmd_plus_args[0], env);
	if (!cmd_path)
	{
		free_cmd_and_path(cmd_plus_args, cmd_path);
		return (-6);
	}
	else
	{
		free_cmd_and_path(cmd_plus_args, cmd_path);
		return (0);
	}
}

/**
 * Used at the early stage of program in functions - "check_args_hdoc" 
 * and "check_args_multi". It explicitly prints an error message 
 * (via "no_cmd_path") if the command cannot be found.
 */
int	check_cmd(char *argument, char *env[])
{
	char	**cmd_plus_args;
	char	*cmd_path;

	cmd_plus_args = ft_split(argument, ' ');
	if (!cmd_plus_args)
		print_stderr("malloc");
	if (access(cmd_plus_args[0], F_OK | X_OK) == 0)
	{
		ft_freestr(cmd_plus_args);
		return (5);
	}
	cmd_path = get_path(cmd_plus_args[0], env);
	if (!cmd_path)
	{
		no_cmd_path(cmd_plus_args);
		return (0);
	}
	else
	{
		ft_freestr(cmd_plus_args);
		free(cmd_path);
		return (5);
	}
}
