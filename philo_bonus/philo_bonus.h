/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:31:55 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 13:36:54 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <stdbool.h>
# include <stdint.h>
# include <semaphore.h>

# define FORK_SEM "/forks_sem"
# define WRITE_SEM "/write_sem"
# define DEAD_SEM "/dead_sem"
# define MEAL_SEM "/meal_sem"

typedef struct s_philo
{
	pid_t			pid;
	int				id;
	int				eat_count;
	uint64_t		last_meal;
	struct s_table	*table;
}	t_philo;

typedef struct s_table
{
	int			n_philo;
	uint64_t	time_to_die;
	uint64_t	time_to_eat;
	uint64_t	time_to_sleep;
	int			max_meals;
	uint64_t	start_time;
	sem_t		*forks;
	sem_t		*write_lock;
	sem_t		*dead_lock;
	sem_t		*meal_lock;
	t_philo		*philos;
}	t_table;

#endif