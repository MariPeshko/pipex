/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 23:22:29 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/08 19:13:37 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <fcntl.h>    // open
# include <stddef.h>   // for NULL
# include <stdio.h>    // perror
# include <stdlib.h>   // malloc, free, exit, EXIT_FAILURE, access, dup2, execve
# include <sys/wait.h> // waitpid, pid_t, WIFEXITED(status)
# include <unistd.h>   // write, read (neeeded?), pipe, fork, unlink

/**
 * This structure is part of the 'child_return' structure, which manages 
 * a collection of child process IDs.
 * 
 * This structure represents a node in a linked list, where each node stores
 * a process ID (pid) and a pointer to the next node.
 * @param id: The process ID of a child process.
 * @param next: A pointer to the next node in the list.
*/
typedef struct id_list
{
	pid_t			id;
	struct id_list	*next;
}					t_id_list;

/**
 * This structure is used to manage child processes by storing a linked list
 * of process IDs and a file descriptor. 
 * 
 * @param fd_to_read represents a file descriptor used for reading data
 * from the child processes.
 * @param fd_to_write a FD for the final output of the pipex
*/
typedef struct child_return
{
	t_id_list			*list;
	int				fd_to_read;
	int				fd_to_write;
} child_return ;

// bonus main functions
void				here_doc(int argc, char **argv, char **env);
void				multi_pipe(int argc, char **argv, char **env);
void				process_command(int cmd, char **argv, char **env, 
	struct child_return **ch);
void				child_process(char *argv, char **env, child_return **ch);
void				last_child(char *argv, char **env, child_return **ch, int write_to);
int					w_waitpid(child_return **ch);

// checker
int					check_cmd(char *cmd, char *env[]);
int					acces_cmd(char *argument, char *env[]);

// retrieve a path to command and execute
void				call_cmd(char *argument, char *env[]);
char				*get_path(char *cmd_name, char **env);
char				*get_all_folders(const char *env_var_path, char **env);
char				*exe_exists(char **folders, char *cmd_name);
char				*build_cmd_path(const char *folder, const char *cmd_name);

// files
int					open_infile(char *name_file);
int					open_outfile(char *name_file, char mode);
int					dev_null(int read_from);

// function for struct
child_return		*create_child_return(int read_from);
t_id_list				*node_birth(pid_t pid);
void				list_birth(t_id_list **lst, t_id_list *node);
t_id_list				*ft_lstlast(t_id_list *lst);
void				free_struct(child_return **ch);

// checker and wrappers for fork and pipe
void				check_args_multi(int argc, char **argv, char *env[]);
void				check_args_hdoc(int argc, char **argv, char *env[]);
void				w_errfork_close(int open_fd, int *pipe_fd);
void				w_errpipe_close(int open_fd);

// errors
void				print_stderr(char *what_error);
void				perror_and_exit(char *what_error, int *pipe_fd);
void				no_cmd_path(char **cmd_plus_args);
void				error_file(char *outfile);
void				err_empty_arg(void);
void				how_execute_pipex(void);

// clean up
void				ft_freestr(char **lst);
void				free_cmd_and_path(char **cmd_plus_args, char *cmd_path);

// wrappers for dup2 and close
void				w_dup2(int dupfd, int newfd, int fd_open);

// get_next_line
void				*ft_b_calloc(size_t count, size_t size);
char				*ft_storage_plus_buffer(char *storage, char *buff);
char				*read_until_newline_or_eof(int fd, char *storage);
char				*ft_extract_line(char *storage);
char				*ft_trim_until_newline(char *storage);
char				*get_next_line(int fd);

#endif