/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:43:24 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:19:27 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# include <stdint.h>
# include <errno.h>
# include <stdbool.h>
# include <sys/types.h>

// massimo numero di filosofi
# define MAX_PHILO 200

//messaggi di errore
# define WRONG_ARGV "Error: Invalid input 🥶\n"
# define WRONG_TIME "Error: Invalid time value, must be > 60 and < int max 🤯\n"
# define NEGATIVE "Error: Negative numbers not allowed 😡\n"
# define MAX "Error: Number exceeds INT_MAX 🥵\n"
# define NAN "Error: Found a non number 🥵\n"

//azioni dei filosofi da stampare
# define EAT "is eating 🍝\n"
# define SLEEP "is sleeping 😴\n"
# define THINK "is thinking 🧠\n"
# define TAKE_FORK "has taken a fork 🍴\n"
# define DIE "died 👻\n"
# define N_MEALS "has eaten enough meals!\n"
# define ALL_FINISHED_MSG "All philosophers have eaten enough meals! 🎉"

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_opcode;

typedef enum e_simulation_state
{
	RUNNING,
	END,
}	t_simulation_state;

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	struct s_table	*table;
	int				eat_count;
	int				status;
	int				eating;
	int64_t			time_to_die;
	pthread_t		philo_thread;
	pthread_mutex_t	philo_lock;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}	t_philo;

typedef struct s_table
{
	pthread_t			*tid;
	int					n_philo;
	u_int64_t			time_to_die;
	u_int64_t			time_to_eat;
	u_int64_t			time_to_sleep;
	int					max_meals;
	int					dead;
	u_int64_t			start_time;
	t_simulation_state	sim_state;
	bool				end;
	t_philo				*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		table_lock; //(lock)
	pthread_mutex_t		write_lock;
}	t_table;

// Funzioni di inizializzazione
void		input_init(t_table *table, int argc, char **argv);
void		init_table(t_table *table);
void		input_check(t_table *table);

// Funzioni di utilità
void		*safe_malloc(size_t size);
int			safe_mutex(t_opcode opcode, pthread_mutex_t *mutex);
int			safe_thread(pthread_t *thread, void *(*func)(void *), void *arg,
				t_opcode op);
void		error_exit(char *str);
uint64_t	get_time(void);
void		print_status(t_philo *philo, char *msg);

// Funzioni principali dei thread
void		*philo_routine(void *arg);
void		*supervisor(void *arg);
void		*meals_monitor(void *arg);

// Funzioni di simulazione e cleanup
int			start_simulation(t_table *table);
void		cleanup_table(t_table *table);

// Funzioni di utility che potrebbero essere necessarie (es: ft_atoi)
int			ft_atoi(const char *str);
void		set_bool(pthread_mutex_t *mutex, bool *var, bool value);
bool		get_bool(pthread_mutex_t *mutex, bool *var);
void		set_simulation_state(t_table *table, t_simulation_state new_state);
void		print_meals_status(t_table *table);
void		print_status(t_philo *philo, char *msg);

#endif