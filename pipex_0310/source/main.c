/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeshko <mpeshko@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:20:13 by mpeshko           #+#    #+#             */
/*   Updated: 2024/10/07 16:20:22 by mpeshko          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/pipex.h"
#include "libft/libft.h"

int	main(int argc, char **argv, char **env)
{
	if (argc < 5)
		how_execute_pipex();
	else if (ft_strncmp(argv[1], "here_doc", ft_strlen(argv[1])) == 0)
		here_doc(argc, argv, env);
	else
		multi_pipe(argc, argv, env);
	return (0);
}
