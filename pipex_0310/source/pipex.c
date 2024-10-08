/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 17:11:17 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/08 18:41:03 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

// Function to handle child processes
void	process_command(int cmd, char **argv, char **env, 
	struct child_return **ch)
{
	if (acces_cmd(argv[cmd], env) == -6)
		(*ch)->fd_to_read = dev_null((*ch)->fd_to_read);
	else
		child_process(argv[cmd], env, ch);
}

// Function to handle the last child processes
static void	process_last_command(int cmd, char **argv, char **env, 
	struct child_return **ch)
{
	int	write_to;

	write_to = (*ch)->fd_to_write;
	if (acces_cmd(argv[cmd], env) == -6)
		(*ch)->fd_to_read = dev_null((*ch)->fd_to_read);
	else
		last_child(argv[cmd], env, ch, write_to);
}

/**
 * Determines the starting command based on the accessibility 
 * of the input file.
 * 
 * If there is no read access to the infile, the function returns 3, 
 * indicating that execution should start from the 2nd command (argv[3])
 * as per bash behavior. If read access to the infile is available, 
 * it returns 2, indicating that execution should start from 
 * the 1st command (argv[2]).
 */
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
 * @brief Executes a sequence of commands connected by pipes, replicating 
 * bash-like pipeline behavior for multi-command processing.
 * 
 * The function handles the input and output redirection from files, sets 
 * up pipes between commands, and executes each command sequentially. It 
 * takes in the format: "./pipex infile cmd1 cmd2 ... cmdn outfile".
 * 
 * 1. Opens the input file (infile) and redirects its content to STDIN.
 * 2. Starts executing commands in a pipeline, creating child processes 
 * for each.
 * 3. For each command, it sets up pipes, ensuring that the output of 
 * one command is passed as input to the next.
 * 4. Opens the output file (outfile) for the last command in the pipeline, 
 * redirecting the final output to it. If the open_outfile function 
 * fails (write_to == -1), then the last_child function will not be executed. 
 * 5. Waits for all child processes to finish and frees allocated memory.
 * 
 * @param argc Number of arguments, representing commands and input/output 
 * files.
 * @param argv Array of command arguments in the format: 
 * infile cmd1 cmd2 ... cmdn outfile.
 * @param env  Environment variables for command execution.
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
		process_command(cmd++, argv, env, &ch);
	write_to = open_outfile(argv[argc - 1], 'p');
	if (write_to != -1)
	{
		ch->fd_to_write = write_to;
		process_last_command(cmd, argv, env, &ch);
	}
	if (ch->list != NULL)
		w_waitpid(&ch);
	free_struct(&ch);
}
