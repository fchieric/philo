/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:54:59 by fabi              #+#    #+#             */
/*   Updated: 2024/11/10 18:54:35 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//fatta

/*
static void init_philos(t_table *table)
{
    int i;

    i = 0;
    t_philo *philos  = table->philos;
    while (i < table->n_philo)
    {
        philos[i].id = i + 1;
        philos[i].table = table;
        philos[i].eat_count = 0;
        philos[i].status = 0;
        philos[i].eating = 0;
        philos[i].time_to_die = get_time();
        safe_mutex(INIT, &table->philos[i].philo_lock);
        // Assegna le forchette ai filosofi
        philos[i].r_fork = &table->forks[i];
        philos[i].l_fork = &table->forks[(i + 1) % table->n_philo];
        i++;
    }
}
*/

static void init_philos(t_table *table)
{
    int i;
    
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

static void init_forks(t_table *table)
{
    int i;

    i = 0;
    while (i < table->n_philo)
    {
        safe_mutex(INIT, &table->forks[i]);
        i++;
    }
}

// Funzione principale di inizializzazione
void init_table(t_table *table)
{
    // Alloca memoria per i thread
    table->tid = safe_malloc(sizeof(pthread_t) * table->n_philo);
    // Alloca memoria per i filosofi
    table->philos = safe_malloc(sizeof(t_philo) * table->n_philo);
    // Alloca memoria per le forchette
    table->forks = safe_malloc(sizeof(pthread_mutex_t) * table->n_philo);
    
    // Inizializza i mutex del tavolo
    safe_mutex(INIT, &table->table_lock);
    safe_mutex(INIT, &table->write_lock);
    
    // Inizializza altre variabili del tavolo
    table->dead = 0;
    table->end = 0;
    table->start_time = 0;
    
    // Inizializza forchette e filosofi
    init_forks(table);
    init_philos(table);
}

void   input_check(t_table *table)
{
    if (table->n_philo > MAX_PHILO)
    {
        error_exit(WRONG_ARGV);
    }
    if ((table->time_to_die < 60 || table->time_to_die > INT_MAX) || (table->time_to_eat < 60
        || table->time_to_eat > INT_MAX) || (table->time_to_sleep < 60 || table->time_to_sleep > INT_MAX))
    {
        error_exit(WRONG_TIME);
    }
    if (table->max_meals >= INT_MAX || (table->max_meals == 0 || table->max_meals < -1))
    {
        if (table->max_meals == INT_MAX)
            error_exit(MAX);
        else
            error_exit(NEGATIVE);
    }
}

void input_init(t_table *table, int argc, char **argv)
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