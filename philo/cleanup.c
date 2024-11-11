/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:52:55 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 13:09:55 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		safe_thread(&table->tid[i], NULL, NULL, JOIN);
		i++;
	}
	usleep(1000);
	i = 0;
	while (i < table->n_philo)
	{
		safe_mutex(DESTROY, &table->forks[i]);
		safe_mutex(DESTROY, &table->philos[i].philo_lock);
		i++;
	}
	safe_mutex(DESTROY, &table->table_lock);
	safe_mutex(DESTROY, &table->write_lock);
	if (table->tid)
		free(table->tid);
	if (table->philos)
		free(table->philos);
	if (table->forks)
		free(table->forks);
}
