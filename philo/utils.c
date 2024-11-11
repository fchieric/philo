/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:59:28 by fabi              #+#    #+#             */
/*   Updated: 2024/11/05 13:28:11 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void error_exit(char *message)
{
    printf("%s\n", message);
    exit(1);
}


uint64_t get_time(void)
{
    struct timeval tv;
    
    if (gettimeofday(&tv, NULL) != 0)
    {
        perror("gettimeofday failed");
        return -1;  // Valore di errore
    }
    return ((tv.tv_sec * (uint64_t)1000) + (tv.tv_usec / 1000));
}

