/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:59:28 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:04:56 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_exit(char *message)
{
	printf("%s\n", message);
	exit(1);
}

uint64_t	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) != 0)
	{
		perror("gettimeofday failed");
		return (-1);
	}
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void	print_meals_status(t_table *table)
{
	int	i;

	i = 0;
	printf("\nMeals status:\n");
	while (i < table->n_philo)
	{
		safe_mutex(LOCK, &table->philos[i].philo_lock);
		printf("Philosopher %d has eaten %d meals\n",
			table->philos[i].id,
			table->philos[i].eat_count);
		safe_mutex(UNLOCK, &table->philos[i].philo_lock);
		i++;
	}
	printf("\n");
}

void	print_status(t_philo *philo, char *msg)
{
	uint64_t	current_time;

	safe_mutex(LOCK, &philo->table->write_lock);
	if (!philo->table->dead && !philo->table->end)
	{
		current_time = get_time() - philo->table->start_time;
		printf("%lu %d %s", current_time, philo->id, msg);
	}
	safe_mutex(UNLOCK, &philo->table->write_lock);
}
