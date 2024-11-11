/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:53:46 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:23:14 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_bool(pthread_mutex_t *mutex, bool *var, bool value)
{
	pthread_mutex_lock(mutex);
	*var = value;
	pthread_mutex_unlock(mutex);
}

bool	get_bool(pthread_mutex_t *mutex, bool *var)
{
	bool	value;

	pthread_mutex_lock(mutex);
	value = *var;
	pthread_mutex_unlock(mutex);
	return (value);
}

void	set_simulation_state(t_table *table, t_simulation_state new_state)
{
	safe_mutex(LOCK, &table->table_lock);
	table->sim_state = new_state;
	safe_mutex(UNLOCK, &table->table_lock);
}

t_simulation_state	get_simulation_state(t_table *table)
{
	t_simulation_state	state;

	safe_mutex(LOCK, &table->table_lock);
	state = table->sim_state;
	safe_mutex(UNLOCK, &table->table_lock);
	return (state);
}
