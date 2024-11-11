/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:54:59 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:26:38 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		table->philos[i].id = i + 1;
		table->philos[i].table = table;
		table->philos[i].eat_count = 0;
		table->philos[i].status = 0;
		table->philos[i].eating = 0;
		table->philos[i].time_to_die = get_time();
		safe_mutex(INIT, &table->philos[i].philo_lock);
		table->philos[i].r_fork = &table->forks[i];
		table->philos[i].l_fork = &table->forks[(i + 1) % table->n_philo];
		i++;
	}
}

static void	init_forks(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philo)
	{
		safe_mutex(INIT, &table->forks[i]);
		i++;
	}
}

void	init_table(t_table *table)
{
	table->tid = safe_malloc(sizeof(pthread_t) * table->n_philo);
	table->philos = safe_malloc(sizeof(t_philo) * table->n_philo);
	table->forks = safe_malloc(sizeof(pthread_mutex_t) * table->n_philo);
	safe_mutex(INIT, &table->table_lock);
	safe_mutex(INIT, &table->write_lock);
	table->dead = 0;
	table->end = 0;
	table->start_time = 0;
	init_forks(table);
	init_philos(table);
}

void	input_check(t_table *table)
{
	if (table->n_philo > MAX_PHILO)
	{
		error_exit(WRONG_ARGV);
	}
	if ((table->time_to_die < 60 || table->time_to_die > INT_MAX)
		|| (table->time_to_eat < 60 || table->time_to_eat > INT_MAX)
		|| (table->time_to_sleep < 60 || table->time_to_sleep > INT_MAX))
	{
		error_exit(WRONG_TIME);
	}
	if (table->max_meals >= INT_MAX || (table->max_meals == 0
			|| table->max_meals < -1))
	{
		if (table->max_meals == INT_MAX)
			error_exit(MAX);
		else
			error_exit(NEGATIVE);
	}
}

void	input_init(t_table *table, int argc, char **argv)
{
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->max_meals = ft_atoi(argv[5]);
	else
		table->max_meals = -1;
	input_check(table);
	init_table(table);
}
