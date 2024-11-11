/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner_check.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:55:52 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:59:44 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_philos_meals(t_table *table, int *all_finished)
{
	int	i;

	i = 0;
	while (i < table->n_philo && table->max_meals != -1)
	{
		safe_mutex(LOCK, &table->philos[i].philo_lock);
		if (table->philos[i].eat_count < table->max_meals)
		{
			*all_finished = 0;
			safe_mutex(UNLOCK, &table->philos[i].philo_lock);
			break ;
		}
		safe_mutex(UNLOCK, &table->philos[i].philo_lock);
		i++;
	}
	if (i == table->n_philo && *all_finished
		&& table->max_meals != -1 && !table->end)
	{
		table->end = 1;
		safe_mutex(LOCK, &table->write_lock);
		printf("%lu %s\n", get_time() - table->start_time, ALL_FINISHED_MSG);
		print_meals_status(table);
		safe_mutex(UNLOCK, &table->write_lock);
	}
	safe_mutex(UNLOCK, &table->table_lock);
}

void	*meals_monitor(void *arg)
{
	t_table	*table;
	int		all_finished;

	table = (t_table *)arg;
	while (!table->dead && !table->end)
	{
		all_finished = 1;
		safe_mutex(LOCK, &table->table_lock);
		check_philos_meals(table, &all_finished);
		if (!all_finished && !table->end)
			usleep(1000);
	}
	return (NULL);
}

void	*supervisor(void *arg)
{
	t_philo		*philo;
	uint64_t	current_time;

	philo = (t_philo *)arg;
	while (!philo->table->dead && !philo->table->end)
	{
		safe_mutex(LOCK, &philo->philo_lock);
		current_time = get_time();
		if (!philo->eating
			&& (current_time - philo->time_to_die > philo->table->time_to_die))
		{
			print_status(philo, DIE);
			safe_mutex(LOCK, &philo->table->table_lock);
			philo->table->dead = 1;
			safe_mutex(UNLOCK, &philo->table->table_lock);
			safe_mutex(UNLOCK, &philo->philo_lock);
			break ;
		}
		safe_mutex(UNLOCK, &philo->philo_lock);
		usleep(1000);
	}
	return (NULL);
}
