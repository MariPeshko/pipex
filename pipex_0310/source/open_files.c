/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 15:23:25 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/07 15:26:16 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

/**
 * @brief Used to discard input when no valid command is provided, 
 * preventing unnecessary input processing from the original file descriptor.
 * 
 * The "dev_null" function is used when the command in "acces_cmd" function
 * is invalid.
 * 
 * In case of an issue with the command, the program redirects the 
 * input source (fd_to_read) to /dev/null. So there's no input to process, 
 * and it discards anything that might have been read from the original 
 * file descriptor.
 */
int	dev_null(int read_from)
{
	int	new_read_from;

	close(read_from);
	new_read_from = open("/dev/null", O_RDONLY);
	return (new_read_from);
}

/**
 * @brief This function displays an error message when there is a permission
 * issue while trying to access or open a file.
 * 
 * @param name_file: The name of the file that lacks the necessary
 * permissions.
 */
static void	error_permission(char *name_file)
{
	ft_putstr_fd("pipex: ", STDERR_FILENO);
	ft_putstr_fd(name_file, STDERR_FILENO);
	ft_putstr_fd(": Permission denied", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

/**
 * @brief Opens a file for reading. If the file does not exist or cannot 
 * be accessed, it handles the errors accordingly. If the file cannot 
 * be opened, it defaults to opening /dev/null.
 */
int	open_infile(char *name_file)
{
	int	file_in;

	file_in = open(name_file, O_RDONLY);
	if (access(name_file, F_OK) != 0)
		error_file(name_file);
	else if (access(name_file, R_OK) != 0)
		error_permission(name_file);
	if (file_in == -1)
		file_in = open("/dev/null", O_RDONLY);
	return (file_in);
}

/**
 * Opens or creates a file for writing. It handles two modes: 'p' for 
 * truncating the file (overwrite mode) and 'h' for appending to the file. 
 * If there are permission issues or file errors, the function manages 
 * those with appropriate error handlers.
 * 
 * The 0644 in the call to open represents the file permissions.
 * 0: This leading 0 indicates that the number is in octal (base 8).
 * 6 in octal is equivalent to 4 (read) + 2 (write), meaning the owner 
 * has read and write permissions.
 * 4 in octal corresponds to read permission. So the group can only 
 * read the file.
 * 4 (others): This is the permission for others. Others can only read.
 */
int	open_outfile(char *name_file, char mode)
{
	int	file_out;

	file_out = 0;
	if (mode == 'p')
		file_out = open(name_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (mode == 'h')
		file_out = open(name_file, O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (access(name_file, W_OK) != 0)
	{
		error_permission(name_file);
		return (-1);
	}
	if (file_out == -1)
	{
		error_file(name_file);
		return (-1);
	}
	return (file_out);
}
