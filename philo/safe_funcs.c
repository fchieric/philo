/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:31:03 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:29:58 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
	{
		error_exit("Error: Malloc failed :(\n");
	}
	return (ret);
}

void	mutex_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if (status == EINVAL)
	{
		if (opcode == INIT)
			error_exit("Error: Invalid attribute value :(\n");
		else
			error_exit("Error: Invalid mutex value :(\n");
	}
	else if (status == EDEADLK)
		error_exit("Error: A deadlock condition was detected :(\n");
	else if (status == EAGAIN)
		error_exit("Error: Max number of recursive locks exceeded :(\n");
	else if (status == ENOMEM)
		error_exit("Error: Insufficient memory for mutex initialization :(\n");
	else if (status == EBUSY)
		error_exit("Error: The mutex is already locked :(\n");
	else if (status == EPERM)
		error_exit("Error: Current thread doesn't hold a lock on mutex :(\n");
	else
		error_exit("Error: Mutex operation failed :(\n");
}

int	safe_mutex(t_opcode opcode, pthread_mutex_t *mutex)
{
	int	status;

	status = 0;
	if (opcode == LOCK)
		status = pthread_mutex_lock(mutex);
	else if (opcode == UNLOCK)
		status = pthread_mutex_unlock(mutex);
	else if (opcode == INIT)
		status = pthread_mutex_init(mutex, NULL);
	else if (opcode == DESTROY)
		status = pthread_mutex_destroy(mutex);
	else
		error_exit("Error: Invalid opcode :(\n");
	mutex_error(status, opcode);
	return (status);
}

static void	thread_error(int status, t_opcode op)
{
	if (status == 0)
		return ;
	if (status == EINVAL)
	{
		if (op == JOIN || op == DETACH)
			error_exit("Error: Invalid thread or is not joinable :(\n");
		else
			error_exit("Error: Invalid thread attributes :(\n");
	}
	else if (status == EPERM)
		error_exit("Error: No permission to join or detach the thread :(\n");
	else if (status == ESRCH)
		error_exit("Error: No thread with the ID could be found :(\n");
	else if (status == EDEADLK)
		error_exit("Error: A deadlock was detected :(\n");
	else
		error_exit("Error: Thread operation failed :(\n");
}

int	safe_thread(pthread_t *thread, void *(*func)(void *),
	void *arg, t_opcode op)
{
	int	status;

	status = 0;
	if (op == CREATE)
		status = pthread_create(thread, NULL, func, arg);
	else if (op == JOIN)
		status = pthread_join(*thread, NULL);
	else if (op == DETACH)
		status = pthread_detach(*thread);
	else
		error_exit("Error: Invalid thread operation :(\n");
	thread_error(status, op);
	return (status);
}
