/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 20:11:23 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/07 20:13:30 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

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
	else
	{
		node = node_birth(pid);
		list_birth(&(*ch)->list, node);
		close(fd[1]);
		dup2((*ch)->fd_to_read, STDIN_FILENO);
		close((*ch)->fd_to_read);
	}
}

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
	else
	{
		node = node_birth(pid);
		list_birth(&(*ch)->list, node);
		close(fd[1]);
		dup2((*ch)->fd_to_read, STDIN_FILENO);
		close((*ch)->fd_to_read);
		(*ch)->fd_to_read = fd[0];
	}
}
