/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 15:46:05 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/07 15:38:34 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

/**
 * @brief Prints an error message to standard error when a command argument 
 * is empty.
 * Error message indicats that an empty command ('') was found and couldn't 
 * be executed. The message is formatted to resemble a shell error.
 */
void	err_empty_arg(void)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd("Command '' not found\n", STDERR_FILENO);
}

/**
 * @brief This function displays an error message when the specified
 * output file cannot be found or opened.
 * 
 * @param outfile: The name of the output file that could not be found.
 */
void	error_file(char *outfile)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(outfile, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

/**
 * Prints an error message to standard error when a command is not found 
 * in the system's executable paths.
 * 
 * @param cmd_plus_args[0] The first element of **cmd_plus_args 
 * is the command.
 */
void	no_cmd_path(char **cmd_plus_args)
{
	ft_putstr_fd("pipex: command not found: ", STDERR_FILENO);
	ft_putstr_fd(cmd_plus_args[0], STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	ft_freestr(cmd_plus_args);
}

/**
 * Prints a message to standard error describing the error associated 
 * with the given argument, based on the current value of errno.
 * 
 * The perror() function prints a message to stderr that describes 
 * the last error that occurred based on the global variable errno.
 */
void	print_stderr(char *what_error)
{
	perror(what_error);
}

/**
 * Prints an error message based on errno and then exits the program 
 * with failure, closing the pipe file descriptors if provided.
 * 
 * Uses perror to print a system error message related to the 
 * last error (stored in errno), along with the custom message.
 */
void	perror_and_exit(char *what_error, int *pipe_fd)
{
	perror(what_error);
	if (pipe_fd)
	{
		if (close(pipe_fd[0]) == -1)
			perror("Error closing pipe [0]");
		if (close(pipe_fd[1]) == -1)
			perror("Error closing pipe [1]");
	}
	exit (EXIT_FAILURE);
}
