/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_here_doc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 14:31:03 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/08 18:52:29 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

/**
 * @brief Reads user input from the terminal and writes it to the file
 * until the limiter string is entered.
 * 
 * This function enters an infinite loop. It reads lines using get_next_line(0)
 * (from standard input), and checks if the line matches the limiter.
 * The condition 'str[ft_strlen(lim)] == 10' checks whether the next
 * character after the word LIMITER is '\n' which is 10 in ASCII.
 * 
 * @param write_end: File descriptor for the file being written to.
 * @param lim: The delimiter string that marks the end of the input.
 */
static void	here_read_helper(int write_end, char *lim)
{
	char	*str;

	while (1)
	{
		str = get_next_line(0);
		if (ft_strncmp(str, lim, ft_strlen(lim)) == 0
			&& str[ft_strlen(lim)] == 10)
		{
			free(str);
			exit(EXIT_SUCCESS);
		}
		write(write_end, str, ft_strlen(str));
		free(str);
	}
}

// The function reads from the standart input with get_next_line
// and writes to the pipe to fd[1]. When the child process reads the LIMITER
// it exits and gives a signal to parent process that it finished successfully.
static int	here_read(char *lim)
{
	int		pip;
	int		id;
	int		fd[2];

	pip = pipe(fd);
	pip++;
	id = fork();
	if (id == 0)
	{
		close(fd[0]);
		here_read_helper(fd[1], lim);
	}
	close(fd[1]);
	waitpid(id, NULL, 0);
	return (fd[0]);
}

/**
 * @brief reads from the heredoc input and executing the commands 
 * sequentially, redirecting output to the outfile in 
 * appending mode.
 * 
 * It simulates the input of bash
 * cmd1 << LIMITER | cmd2 >> file
 * 
 * Example of input
 * ./pipex here_doc LIMITER cmd1 cmd2 file
 * 
 * @param mode 'h' is a APPEND mode for outfile
 * @param cmd The first command (cmd1) in heredoc mode starts 
 * at the fourth argument, hence cmd is initialized to 3.
 */
void	here_doc(int argc, char **argv, char **env)
{
	int					cmd;
	char				mode;
	int					write_to;
	int					read_from;
	struct child_return	*ch;

	check_args_hdoc(argc, argv, env);
	read_from = here_read(argv[2]);
	w_dup2(read_from, STDIN_FILENO, -2);
	ch = create_child_return(read_from);
	mode = 'h';
	cmd = 3;
	while (cmd < argc - 2)
		process_command(cmd++, argv, env, &ch);
	write_to = open_outfile(argv[argc - 1], mode);
	if (write_to != -1)
		last_child(argv[cmd], env, &ch, write_to);
	if (ch->list != NULL)
		w_waitpid(&ch);
	free_struct(&ch);
}
