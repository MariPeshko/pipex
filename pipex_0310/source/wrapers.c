/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrapers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 14:30:17 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/06 15:55:22 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"
/**
 * The wrapers.c file contains three utility functions designed to wrap common
 * system calls (dup2, fork, and pipe) and provide consistent error handling. 
*/
/**
 * @brief This function is a wrapper around the dup2 system call.
 * It attempts to duplicate dupfd onto newfd. If dup2 fails, it handles
 * the error by closing the fd_open file descriptor (if it's not -2), 
 * printing an error message, and terminating the program.
 * The function ensures that the original file descriptor (dupfd) is
 * closed whether the operation succeeds or fails.
 * 
 * @param dupfd: The file descriptor to duplicate.
 * @param newfd: The target file descriptor where dupfd will be duplicated.
 * @param fd_open: An additional file descriptor that will be closed on error,
 * unless its value is -2, which means it will be skipped.
*/
void	w_dup2(int dupfd, int newfd, int fd_open)
{
	if (dup2(dupfd, newfd) == -1)
	{
		if (fd_open != -2)
			close(fd_open);
		print_stderr("dup2");
		close(dupfd);
		exit(EXIT_FAILURE);
	}
	else
		close(dupfd);
}

/**
 * @brief This function handles errors during a fork operation.
 * When fork fails, it prints an error message, closes the file descriptor
 * "open_fd", closes both ends of a pipe (given by pipe_fd), and exits 
 * the program.
*/
void	w_errfork_close(int open_fd, int *pipe_fd)
{
	perror("Fork Error");
	close(open_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exit(EXIT_FAILURE);
}

/**
 * @brief This function handles errors related to pipe creation.
 * It closes the provided file descriptor open_fd, prints an error message
 * related to pipe, and exits the program.
*/
void	w_errpipe_close(int open_fd)
{
	close(open_fd);
	perror_and_exit("pipe", NULL);
}

/**
 * This function is a wrapper for the waitpid system call, designed to wait
 * for the termination of multiple child processes stored in a linked list.
 * It ensures that each child process in the 'child_return' structure is
 * waited on, handling process management efficiently.
 * 
 * This function iterates through a linked list of process IDs (pids)
 * stored in the child_return structure and calls waitpid on each one
 * to wait for their termination.
 * 
 * @param ch: A double pointer to a child_return structure containing
 * the list of process IDs to wait for.
*/
int	w_waitpid(child_return **ch)
{
	t_id_list *tmp;

	if (!*ch)
		return (-3);
	tmp = (*ch)->list;
	while (tmp)
	{
		waitpid(tmp->id, NULL, 0);
		tmp = tmp->next;
	}
	return(0);
}
