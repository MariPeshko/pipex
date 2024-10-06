/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_struct.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 15:21:30 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/06 12:42:23 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

/**
 * Frees all elements in a linked list of type t_id_list. If the list is
 * already empty, it returns -1, otherwise, it iterates through the list,
 * freeing each node one by one, and returns 0 upon successful completion.
*/
static int	free_list(t_id_list **list)
{
	t_id_list	*tmp;

	if (!*list)
		return (-1);
	while (*list)
	{
		tmp = (*list)->next;
		free(*list);
		*list = tmp;
	}
	return (0);
}

/**
 * Frees the memory allocated for a "child_return" structure and its
 * associated list. This function calls free_list() to free
 * the linked list within the structure, then frees the structure itself
 * and sets the pointer to NULL.
*/
void	free_struct(child_return **ch)
{
	free_list(&(*ch)->list);
	free(*ch);
	(*ch) = NULL;
}

/**
 * Returns the last node in a linked list of type "t_id_list".
 * It iterates through the list until it finds the last node,
 * which is the one with a NULL next pointer.
*/
t_id_list	*ft_lstlast(t_id_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/**
 * Prints an error message ("Error\n") to the standard error output
 * and exits the program with the given error code. This function is
 * used to handle memory allocation errors.
*/
void	error_mall_exit(int err_number)
{
	write(2, "Error\n", 6);
	exit(err_number);
}

/**
 * Adds a new node to the end of a linked list.
 * If the list is empty, the new node becomes the head;
 * otherwise, it appends the node to the last element
 * in the list using ft_lstlast().
*/
void	list_birth(t_id_list **lst, t_id_list *node)
{
	if (!*lst)
		*lst = node;
	else
		(ft_lstlast(*lst))->next = node;
}

/**
 * Allocates memory for a new node of type t_id_list, initializes
 * it with the given process ID (pid), and sets its next pointer to NULL.
 * If memory allocation fails, the function calls error_mall_exit()
 * to handle the error.
 * 
 * @param pid: The process ID to be assigned to the new node.
*/
t_id_list	*node_birth(pid_t pid)
{
	t_id_list *node;

	node = ft_calloc(1, sizeof(t_id_list));
	if (!node)
		error_mall_exit(5);
	node->id = pid;
	node->next = NULL;
	return (node);
}