/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:43:06 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:10:22 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	take_forks(t_philo *philo)
{
	if (get_bool(&philo->table->table_lock, &philo->table->end))
		return ;
	safe_mutex(LOCK, philo->r_fork);
	if (get_bool(&philo->table->table_lock, &philo->table->end))
	{
		safe_mutex(UNLOCK, philo->r_fork);
		return ;
	}
	print_status(philo, TAKE_FORK);
	safe_mutex(LOCK, philo->l_fork);
	if (get_bool(&philo->table->table_lock, &philo->table->end))
	{
		safe_mutex(UNLOCK, philo->l_fork);
		safe_mutex(UNLOCK, philo->r_fork);
		return ;
	}
	print_status(philo, TAKE_FORK);
}

static void	eating(t_philo *philo)
{
	take_forks(philo);
	if (get_bool(&philo->table->table_lock, &philo->table->end))
		return ;
	safe_mutex(LOCK, &philo->philo_lock);
	philo->eating = 1;
	philo->time_to_die = get_time();
	philo->eat_count++;
	print_status(philo, EAT);
	safe_mutex(UNLOCK, &philo->philo_lock);
	usleep(philo->table->time_to_eat * 1000);
	safe_mutex(LOCK, &philo->philo_lock);
	philo->eating = 0;
	safe_mutex(UNLOCK, &philo->philo_lock);
	safe_mutex(UNLOCK, philo->l_fork);
	safe_mutex(UNLOCK, philo->r_fork);
}

void	philo_routine_loop(t_philo *philo)
{
	while (!philo->table->dead && !philo->table->end)
	{
		eating(philo);
		safe_mutex(LOCK, &philo->table->table_lock);
		if (philo->table->max_meals != -1
			&& philo->eat_count >= philo->table->max_meals)
		{
			safe_mutex(UNLOCK, &philo->table->table_lock);
			break ;
		}
		safe_mutex(UNLOCK, &philo->table->table_lock);
		print_status(philo, SLEEP);
		usleep(philo->table->time_to_sleep * 1000);
		print_status(philo, THINK);
	}
}

void	*philo_routine(void *arg)
{
	t_philo		*philo;
	pthread_t	supervisor_thread;

	philo = (t_philo *)arg;
	safe_thread(&supervisor_thread, supervisor, philo, CREATE);
	safe_thread(&supervisor_thread, NULL, NULL, DETACH);
	if (philo->table->n_philo == 1)
	{
		safe_mutex(LOCK, philo->r_fork);
		print_status(philo, TAKE_FORK);
		usleep(philo->table->time_to_die * 1000);
		safe_mutex(UNLOCK, philo->r_fork);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(1000);
	philo_routine_loop(philo);
	return (NULL);
}

int	start_simulation(t_table *table)
{
	int			i;
	pthread_t	monitor;

	table->start_time = get_time();
	if (table->n_philo > 1 && table->max_meals != -1)
	{
		safe_thread(&monitor, meals_monitor, table, CREATE);
		safe_thread(&monitor, NULL, NULL, DETACH);
	}
	i = 0;
	while (i < table->n_philo)
	{
		if (safe_thread(&table->tid[i], philo_routine,
				&table->philos[i], CREATE) != 0)
		{
			table->end = 1;
			return (1);
		}
		i++;
	}
	return (0);
}
