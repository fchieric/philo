/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:57:02 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 11:32:17 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


// Main aggiornato con gestione completa
int main(int argc, char **argv)
{
    t_table table;

    if (argc < 5 || argc > 6)
        error_exit("Error: Invalid number of arguments\n");
    
    // Inizializza la struttura
    input_init(&table, argc, argv);
    
    // Avvia la simulazione
    if (start_simulation(&table) != 0)
    {
        cleanup_table(&table);
        error_exit("Error: Failed to start simulation\n");
    }

    // Attende e pulisce
    cleanup_table(&table);
    return (0);
}



/*
int main(int argc, char **argv)
{
    t_table table;

    if (argc < 5 || argc > 6)
        error_exit("Error: Invalid number of arguments\n");
    input_init(&table, argc, argv);
    
    
    // Assicuriamoci che la simulazione sia terminata
    while (get_simulation_state(&table) == RUNNING)
        usleep(1000);
        
    cleanup_table(&table);
    return (0);
}
*/