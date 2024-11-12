/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:31:37 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 13:38:08 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

uint64_t    get_time(void)
{
	struct timeval tv;
	
	if (gettimeofday(&tv, NULL) != 0)
		return (-1);
	return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

void    print_status(t_philo *philo, char *msg)
{
	sem_wait(philo->table->write_lock);
	printf("%lu %d %s", get_time() - philo->table->start_time, philo->id, msg);
	sem_post(philo->table->write_lock);
}

void    init_semaphores(t_table *table)
{
	sem_unlink(FORK_SEM);
	sem_unlink(WRITE_SEM);
	sem_unlink(DEAD_SEM);
	sem_unlink(MEAL_SEM);
	
	table->forks = sem_open(FORK_SEM, O_CREAT, 0644, table->n_philo);
	table->write_lock = sem_open(WRITE_SEM, O_CREAT, 0644, 1);
	table->dead_lock = sem_open(DEAD_SEM, O_CREAT, 0644, 1);
	table->meal_lock = sem_open(MEAL_SEM, O_CREAT, 0644, 1);
	
	if (table->forks == SEM_FAILED || table->write_lock == SEM_FAILED || 
		table->dead_lock == SEM_FAILED || table->meal_lock == SEM_FAILED)
	{
		printf("Error: semaphore initialization failed\n");
		exit(1);
	}
}

void    *death_monitor(void *arg)
{
	t_philo     *philo;
	uint64_t    current_time;

	philo = (t_philo *)arg;
	while (1)
	{
		sem_wait(philo->table->meal_lock);
		current_time = get_time();
		if (current_time - philo->last_meal > philo->table->time_to_die)
		{
			sem_wait(philo->table->write_lock);
			printf("%lu %d died\n", current_time - philo->table->start_time, philo->id);
			sem_post(philo->table->dead_lock);
			exit(1);
		}
		sem_post(philo->table->meal_lock);
		usleep(1000);
	}
	return (NULL);
}

void    philosopher_routine(t_philo *philo)
{
	pthread_t   monitor;

	philo->last_meal = get_time();
	if (pthread_create(&monitor, NULL, death_monitor, philo) != 0)
	{
		printf("Error: failed to create monitor thread\n");
		exit(1);
	}
	pthread_detach(monitor);

	while (1)
	{
		// Take forks
		sem_wait(philo->table->forks);
		print_status(philo, "has taken a fork\n");
		sem_wait(philo->table->forks);
		print_status(philo, "has taken a fork\n");

		// Eating
		sem_wait(philo->table->meal_lock);
		philo->last_meal = get_time();
		philo->eat_count++;
		print_status(philo, "is eating\n");
		sem_post(philo->table->meal_lock);
		usleep(philo->table->time_to_eat * 1000);

		// Release forks
		sem_post(philo->table->forks);
		sem_post(philo->table->forks);

		if (philo->table->max_meals != -1 && 
			philo->eat_count >= philo->table->max_meals)
			exit(0);

		// Sleep and think
		print_status(philo, "is sleeping\n");
		usleep(philo->table->time_to_sleep * 1000);
		print_status(philo, "is thinking\n");
	}
}

void    start_simulation(t_table *table)
{
	int     i;
	int     status;

	table->start_time = get_time();
	i = 0;
	while (i < table->n_philo)
	{
		table->philos[i].pid = fork();
		if (table->philos[i].pid == -1)
		{
			printf("Error: fork failed\n");
			exit(1);
		}
		if (table->philos[i].pid == 0)
		{
			philosopher_routine(&table->philos[i]);
			exit(0);
		}
		i++;
	}

	// Wait for death or completion
	sem_wait(table->dead_lock);
	i = 0;
	while (i < table->n_philo)
	{
		kill(table->philos[i].pid, SIGTERM);
		i++;
	}
}

void    cleanup(t_table *table)
{
	sem_close(table->forks);
	sem_close(table->write_lock);
	sem_close(table->dead_lock);
	sem_close(table->meal_lock);
	
	sem_unlink(FORK_SEM);
	sem_unlink(WRITE_SEM);
	sem_unlink(DEAD_SEM);
	sem_unlink(MEAL_SEM);
	
	free(table->philos);
}

int main(int argc, char **argv)
{
	t_table table;

	if (argc < 5 || argc > 6)
	{
		printf("Error: wrong number of arguments\n");
		return (1);
	}

	table.n_philo = atoi(argv[1]);
	table.time_to_die = atoi(argv[2]);
	table.time_to_eat = atoi(argv[3]);
	table.time_to_sleep = atoi(argv[4]);
	table.max_meals = (argc == 6) ? atoi(argv[5]) : -1;

	table.philos = malloc(sizeof(t_philo) * table.n_philo);
	if (!table.philos)
		return (1);

	init_semaphores(&table);
	
	for (int i = 0; i < table.n_philo; i++)
	{
		table.philos[i].id = i + 1;
		table.philos[i].eat_count = 0;
		table.philos[i].table = &table;
	}

	start_simulation(&table);
	cleanup(&table);
	return (0);
}