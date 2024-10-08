/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 20:11:23 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/08 19:31:36 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

/**
 * It is mstly tha same function as "child_process", but for the last
 * command.
 * 
 * @param write_to is a FD for the output of the whole program.
 */
void	last_child(char *argv, char **env, child_return **ch, int write_to)
{
	pid_t		pid;
	int			fd[2];
	t_id_list	*node;

	if (pipe(fd) == -1)
		w_errpipe_close((*ch)->fd_to_read);
	pid = fork();
	if (pid == -1)
		w_errfork_close((*ch)->fd_to_read, fd);
	if (pid == 0)
	{
		close(fd[0]);
		w_dup2(write_to, STDOUT_FILENO, -2);
		close(write_to);
		if (!*argv)
			exit(EXIT_FAILURE);
		call_cmd(argv, env);
	}
	node = node_birth(pid);
	list_birth(&(*ch)->list, node);
	close(fd[1]);
	dup2((*ch)->fd_to_read, STDIN_FILENO);
	close((*ch)->fd_to_read);
}

/**
 * @brief Handles the creation and execution of a child process.
 * 
 * This function creates a pipe and forks a new child process.
 * The child process redirects its standard output (STDOUT) to 
 * the write end of the pipe, and if the command is valid, it executes it.
 * The parent process updates the read end of the pipe, stores 
 * the child process information in a list for later use in `w_waitpid`
 * function, and redirects the input to the previous process’s output.
 * 
 * If any error occurs during pipe or fork creation, it handles 
 * the error and closes relevant file descriptors.
 * 
 * @param argv Command to be executed in the child process.
 * @param env Array of environment variables.
 * @param ch Pointer to a child_return structure storing process details.
 * @param fd_to_read  Each command reads input from `(*ch)->fd_to_read`, 
 * which can be:
 * - A file or heredoc file or /dev/null if input redirection 
 * is being used;
 * - The read end of the pipe from the previous command in a pipeline.
 * The parent process updates `(*ch)->fd_to_read = fd[0]` to ensure 
 * the next command in the pipeline reads from the appropriate input.
 */
void	child_process(char *argv, char **env, child_return **ch)
{
	pid_t		pid;
	int			fd[2];
	t_id_list	*node;

	if (pipe(fd) == -1)
		w_errpipe_close((*ch)->fd_to_read);
	pid = fork();
	if (pid == -1)
		w_errfork_close((*ch)->fd_to_read, fd);
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (!*argv)
			exit(EXIT_FAILURE);
		call_cmd(argv, env);
	}
	node = node_birth(pid);
	list_birth(&(*ch)->list, node);
	close(fd[1]);
	dup2((*ch)->fd_to_read, STDIN_FILENO);
	close((*ch)->fd_to_read);
	(*ch)->fd_to_read = fd[0];
}
