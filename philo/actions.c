/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:43:06 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 11:21:02 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_meals_status(t_table *table)
{
    int i;
    printf("\nMeals status:\n");
    for (i = 0; i < table->n_philo; i++)
    {
        safe_mutex(LOCK, &table->philos[i].philo_lock);
        printf("Philosopher %d has eaten %d meals\n", 
               table->philos[i].id, 
               table->philos[i].eat_count);
        safe_mutex(UNLOCK, &table->philos[i].philo_lock);
    }
    printf("\n");
}


// Funzione per stampare i messaggi di stato
void print_status(t_philo *philo, char *msg)
{
    safe_mutex(LOCK, &philo->table->write_lock);
    if (!philo->table->dead && !philo->table->end)
    {
        uint64_t current_time = get_time() - philo->table->start_time;
        printf("%lu %d %s", current_time, philo->id, msg);
    }
    safe_mutex(UNLOCK, &philo->table->write_lock);
}


void *meals_monitor(void *arg)
{
    t_table *table = (t_table *)arg;
    int i;
    int all_finished;

    while (!table->dead)
    {
        i = 0;
        all_finished = 1;
        safe_mutex(LOCK, &table->table_lock);
        
        // Controlla che TUTTI i filosofi abbiano mangiato abbastanza
        while (i < table->n_philo && table->max_meals != -1)
        {
            safe_mutex(LOCK, &table->philos[i].philo_lock);
            if (table->philos[i].eat_count < table->max_meals)
            {
                all_finished = 0;
                safe_mutex(UNLOCK, &table->philos[i].philo_lock);
                safe_mutex(UNLOCK, &table->table_lock);
                break;
            }
            safe_mutex(UNLOCK, &table->philos[i].philo_lock);
            i++;
        }

        // Se abbiamo controllato tutti i filosofi e tutti hanno finito
        if (i == table->n_philo && all_finished && table->max_meals != -1)
        {
            table->end = 1;
            safe_mutex(UNLOCK, &table->table_lock);
            
            safe_mutex(LOCK, &table->write_lock);
            printf("%lu All philosophers have eaten enough meals!\n", 
                get_time() - table->start_time);
            print_meals_status(table);
            safe_mutex(UNLOCK, &table->write_lock);
            return (NULL);
        }
        
        if (!all_finished)
            usleep(1000);
        else
            safe_mutex(UNLOCK, &table->table_lock);
    }
    return (NULL);
}


// Versione aggiornata del supervisor
void *supervisor(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    uint64_t current_time;
    
    while (!philo->table->dead && !philo->table->end)
    {
        safe_mutex(LOCK, &philo->philo_lock);
        current_time = get_time();
        if (!philo->eating && 
            (current_time - philo->time_to_die > philo->table->time_to_die))
        {
            print_status(philo, DIE);
            safe_mutex(LOCK, &philo->table->table_lock);
            philo->table->dead = 1;
            safe_mutex(UNLOCK, &philo->table->table_lock);
            safe_mutex(UNLOCK, &philo->philo_lock);
            break;
        }
        safe_mutex(UNLOCK, &philo->philo_lock);
        usleep(1000);
    }
    return (NULL);
}

static void eating(t_philo *philo)
{
    // Controllo dello stato `end` prima di prendere le forchette
    if (get_bool(&philo->table->table_lock, &philo->table->end))
        return;

    // Prendere la forchetta destra
    safe_mutex(LOCK, philo->r_fork);
    if (get_bool(&philo->table->table_lock, &philo->table->end)) {
        safe_mutex(UNLOCK, philo->r_fork);
        return; // Terminare se l'esecuzione è conclusa
    }
    print_status(philo, TAKE_FORK);

    // Prendere la forchetta sinistra
    safe_mutex(LOCK, philo->l_fork);
    if (get_bool(&philo->table->table_lock, &philo->table->end)) {
        safe_mutex(UNLOCK, philo->l_fork);
        safe_mutex(UNLOCK, philo->r_fork);
        return; // Terminare se l'esecuzione è conclusa
    }
    print_status(philo, TAKE_FORK);

    // Iniziare a mangiare
    safe_mutex(LOCK, &philo->philo_lock);
    philo->eating = 1;
    philo->time_to_die = get_time();
    philo->eat_count++; // Incrementa il contatore dei pasti
    print_status(philo, EAT);
    safe_mutex(UNLOCK, &philo->philo_lock);

    usleep(philo->table->time_to_eat * 1000);

    // Terminare il pasto
    safe_mutex(LOCK, &philo->philo_lock);
    philo->eating = 0;
    safe_mutex(UNLOCK, &philo->philo_lock);

    // Rilasciare le forchette
    safe_mutex(UNLOCK, philo->l_fork);
    safe_mutex(UNLOCK, philo->r_fork);
}


void *philo_routine(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    pthread_t supervisor_thread;
    
    // Avvia sempre il supervisor, anche per un solo filosofo
    safe_thread(&supervisor_thread, supervisor, philo, CREATE);
    safe_thread(&supervisor_thread, NULL, NULL, DETACH);
    
    // Caso speciale per un solo filosofo
    if (philo->table->n_philo == 1)
    {
        safe_mutex(LOCK, philo->r_fork);
        print_status(philo, TAKE_FORK);
        usleep(philo->table->time_to_die * 1000); // Aspetta di morire
        safe_mutex(UNLOCK, philo->r_fork);
        return (NULL);
    }
    
    if (philo->id % 2 == 0)
        usleep(1000);
    
    while (!philo->table->dead && !philo->table->end)
    {
        eating(philo);
        
        safe_mutex(LOCK, &philo->table->table_lock);
        if (philo->table->max_meals != -1 && 
            philo->eat_count >= philo->table->max_meals)
        {
            safe_mutex(UNLOCK, &philo->table->table_lock);
            break;
        }
        safe_mutex(UNLOCK, &philo->table->table_lock);
        
        print_status(philo, SLEEP);
        usleep(philo->table->time_to_sleep * 1000);
        print_status(philo, THINK);
    }
    return (NULL);
}


// Versione aggiornata della funzione start_simulation
int start_simulation(t_table *table)
{
    int i;
    pthread_t monitor;

    // Se c'è un solo filosofo, non avviare il monitor dei pasti
    // perché il filosofo non potrà mai mangiare
    table->start_time = get_time();
    
    if (table->n_philo > 1 && table->max_meals != -1)
    {
        safe_thread(&monitor, meals_monitor, table, CREATE);
        safe_thread(&monitor, NULL, NULL, DETACH);
    }

    // Avvia tutti i filosofi
    i = 0;
    while (i < table->n_philo)
    {
        if (safe_thread(&table->tid[i], philo_routine, &table->philos[i], CREATE) != 0)
        {
            table->end = 1;  // Segnala a tutti i thread di terminare
            return (1);
        }
        i++;
    }

    return (0);
}