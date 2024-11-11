/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:52:55 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 11:00:50 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void cleanup_table(t_table *table)
{
    int i;

    // Attendi che tutti i filosofi terminino
    i = 0;
    while (i < table->n_philo)
    {
        safe_thread(&table->tid[i], NULL, NULL, JOIN);
        i++;
    }

    // Aspetta un momento per assicurarsi che tutti i messaggi siano stampati
    usleep(1000);

    // Distruggi i mutex delle forchette e dei filosofi
    i = 0;
    while (i < table->n_philo)
    {
        safe_mutex(DESTROY, &table->forks[i]);
        safe_mutex(DESTROY, &table->philos[i].philo_lock);
        i++;
    }

    // Distruggi i mutex del tavolo
    safe_mutex(DESTROY, &table->table_lock);
    safe_mutex(DESTROY, &table->write_lock);

    // Libera la memoria allocata
    if (table->tid)
        free(table->tid);
    if (table->philos)
        free(table->philos);
    if (table->forks)
        free(table->forks);
}
