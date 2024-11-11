/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:53:46 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 10:15:33 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    set_int(pthread_mutex_t *mutex, int *value, int new_value)
{
    pthread_mutex_lock(mutex);
    *value = new_value;
    pthread_mutex_unlock(mutex);
}

int get_int(pthread_mutex_t *mutex, int *value)
{
    int ret;

    pthread_mutex_lock(mutex);
    ret = *value;
    pthread_mutex_unlock(mutex);
    return (ret);
}

void    set_ulong(pthread_mutex_t *mutex, unsigned long *value,
        unsigned long new_value)
{
    pthread_mutex_lock(mutex);
    *value = new_value;
    pthread_mutex_unlock(mutex);
}

unsigned long get_ulong(pthread_mutex_t *mutex, unsigned long *value)
{
    unsigned long ret;

    pthread_mutex_lock(mutex);
    ret = *value;
    pthread_mutex_unlock(mutex);
    return (ret);
}

// Incrementa un intero in modo sicuro utilizzando un mutex (mutex_intincr)
int safe_intplus(pthread_mutex_t *mutex, int *value)
{
    int ret;

    pthread_mutex_lock(mutex);
    *value += 1;
    ret = *value;
    pthread_mutex_unlock(mutex);
    return (ret);
}

void    set_bool(pthread_mutex_t *mutex, bool *var, bool value)
{
    pthread_mutex_lock(mutex);
    *var = value;
    pthread_mutex_unlock(mutex);
}

bool    get_bool(pthread_mutex_t *mutex, bool *var)
{
    bool value;

    pthread_mutex_lock(mutex);
    value = *var;
    pthread_mutex_unlock(mutex);
    return (value);
}

void set_simulation_state(t_table *table, t_simulation_state new_state)
{
    safe_mutex(LOCK, &table->table_lock);
    table->sim_state = new_state;
    safe_mutex(UNLOCK, &table->table_lock);
}

t_simulation_state get_simulation_state(t_table *table)
{
    t_simulation_state state;
    
    safe_mutex(LOCK, &table->table_lock);
    state = table->sim_state;
    safe_mutex(UNLOCK, &table->table_lock);
    return state;
}