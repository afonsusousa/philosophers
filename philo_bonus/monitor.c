/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:16 by amagno-r          #+#    #+#             */
/*   Updated: 2025/08/25 22:48:41 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <signal.h>
#include <sys/wait.h>

void	die(t_data *data, t_phil *phil)
{
	sem_post(data->table.death);
	print_status(data, phil->id, DEATH);
	sem_post(phil->meal_lock);
	sem_wait(data->write_lock);
}
void	*awareness(void *arg)
{
	t_phil	*phil;
	t_data	*data;
	bool	posted_full;

	phil = (t_phil *)arg;
	data = (t_data *)phil->data;
	posted_full = false;
	while (true)
	{
		sem_wait(phil->meal_lock);
		if (!posted_full && phil->meals_eaten >= data->must_eat)
		{
			posted_full = true;
			sem_post(data->table.full);
		}
		if (get_time() - phil->last_meal > data->time_to_die)
		{
			die(data, phil);
			exit(0);
		}
		sem_post(phil->meal_lock);
		usleep(1000);
	}
	return (NULL);
}

void	*meal_watcher(void *arg)
{
	int		count;
	t_data *data;

	data = (t_data *) arg;
	count = data->table.phil_count;
	while (count)
	{
		sem_wait(data->table.full);
		count--;
	}
	kill_all_children(data);
	sem_post(data->table.death);
	return (NULL);
}

void	*death_checker(void *arg) {

	t_data *data;
	int		i;

	data = (t_data *) arg;
	i = data->table.phil_count;
    sem_wait(data->table.death);
	kill_all_children(data);
	while (i--)
		sem_post(data->table.full);
	return (NULL);
}

