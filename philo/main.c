/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:57:02 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:26:55 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc < 5 || argc > 6)
		error_exit("Error: Invalid number of arguments\n");
	input_init(&table, argc, argv);
	if (start_simulation(&table) != 0)
	{
		cleanup_table(&table);
		error_exit("Error: Failed to start simulation\n");
	}
	cleanup_table(&table);
	return (0);
}
