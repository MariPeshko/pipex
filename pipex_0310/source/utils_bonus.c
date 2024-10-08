/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:12:49 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/08 19:39:44 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

void	how_execute_pipex(void)
{
	ft_putstr_fd("Hi, human! Input for execution the pipex:\n",
		STDERR_FILENO);
	ft_putstr_fd("\nMANDATORY PART\n", STDERR_FILENO);
	ft_putstr_fd("./pipex file1 cmd1 | cmd2 > file2\n", STDERR_FILENO);
	ft_putstr_fd("\nBONUS\n", STDERR_FILENO);
	ft_putstr_fd("Multipies:\n./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n",
		STDERR_FILENO);
	ft_putstr_fd("Support « and »\n./pipex here_doc LIMITER cmd cmd1 file\n",
		STDERR_FILENO);
	exit(EXIT_FAILURE);
}

/**
 * @brief Validates the arguments for the heredoc input mode.
 * 
 * This function checks if the correct number of arguments (6) is provided 
 * and ensures that each command is valid and non-empty.
 * If the arguments are invalid or any command 
 * is empty, an error is printed, and the program exits.
 * 
 * Example of valid input:
 * ./pipex_bonus here_doc LIMITER cmd cmd1 file
 * 
 * @param argc Number of arguments passed.
 * @param argv Array of argument strings.
 * @param env Environment variables.
 */
void	check_args_hdoc(int argc, char **argv, char *env[])
{
	int	i;

	i = 3;
	if (argc != 6)
	{
		ft_putstr_fd("Hi, human! Input for execution here_doc:\n",
			STDERR_FILENO);
		ft_putstr_fd("> ./pipex_bonus here_doc LIMITER cmd cmd1 file\n",
			STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	while (i != argc - 1)
	{
		if (!*argv[i])
			err_empty_arg();
		else
			check_cmd(argv[i], env);
		i++;
	}
}

/**
 * @brief Validates the arguments for multiple command execution.
 * 
 * This function checks if the correct number of arguments (at least 5) 
 * is provided for the multi-command execution. It ensures that each command 
 * (cmd1, cmd2, ..., cmdn) is valid and non-empty. If the arguments are 
 * insufficient or any command is empty, an error is printed, and the 
 * program exits.
 * 
 * Example of valid input:
 * ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2
 * 
 * @param argc Number of arguments passed.
 * @param argv Array of argument strings.
 * @param env Environment variables.
 */
void	check_args_multi(int argc, char **argv, char *env[])
{
	int	i;

	i = 2;
	if (argc < 5)
	{
		ft_putstr_fd("Hi, human! Input for execution here_doc:\n",
			STDERR_FILENO);
		ft_putstr_fd("> ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2\n",
			STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	while (i != argc - 1)
	{
		if (!*argv[i])
			err_empty_arg();
		else
			check_cmd(argv[i], env);
		i++;
	}
}
