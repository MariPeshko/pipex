/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:11:17 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/07 16:55:00 by mpeshko          ###   ########.fr       */
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

// If no access to infile, then we start execution not from the 1st
// command, but from the 2nd command. This is a behaviour of bash.
static int	cmd_to_start(char *name_file)
{
	int	file_in;

	file_in = open(name_file, O_RDONLY);
	if (access(name_file, R_OK) != 0)
		return (3);
	else
	{
		close(file_in);
		return (2);
	}
}

// Function to create and initialize a child_return structure.
// Returns the pointer to the newly created structure.
child_return	*create_child_return(int read_from)
{
	child_return	*ch;

	ch = ft_calloc(1, sizeof(child_return));
	if (ch == NULL)
		error_mall_exit(5);
	ch->fd_to_read = read_from;
	ch->list = NULL;
	return (ch);
}

/**
 * 
 * 
 * 
 * In case of an issue with the command, "acces_cmd" function returns -6,
 * the program redirects the input source (fd_to_read) to /dev/null.
 * So there's no input to process, and it discards anything that might
 * have been read from the original file descriptor.
 */
void	multi_pipe(int argc, char **argv, char **env)
{
	int					cmd;
	int					read_from;
	int					write_to;
	struct child_return	*ch;

	check_args_multi(argc, argv, env);
	read_from = open_infile(argv[1]);
	w_dup2(read_from, STDIN_FILENO, -2);
	ch = create_child_return(read_from);
	cmd = cmd_to_start(argv[1]);
	while (cmd < argc - 2)
	{
		if (acces_cmd(argv[cmd], env) == -6)
			ch->fd_to_read = dev_null(ch->fd_to_read);
		else
			child_process(argv[cmd], env, &ch);
		cmd++;
	}
	write_to = open_outfile(argv[argc - 1], 'p');
	if (write_to != -1)
	{
		if (acces_cmd(argv[cmd], env) == -6)
			ch->fd_to_read = dev_null(ch->fd_to_read);
		else
			last_child(argv[cmd], env, &ch, write_to);
	}
	if (ch->list != NULL)
		w_waitpid(&ch);
	free_struct(&ch);
}
