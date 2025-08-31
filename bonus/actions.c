/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 18:36:05 by amagno-r          #+#    #+#             */
/*   Updated: 2025/08/25 22:45:26 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	grab_fork(t_phil *phil)
{
	t_data	*data;

	data = (t_data *)phil->data;
	sem_wait(data->table.forks);
	print_status(data, phil->id, FORK);
}

static void	phil_eat(t_phil *phil)
{
	t_data	*data;

	data = (t_data *)phil->data;
	grab_fork(phil);
	grab_fork(phil);
	sem_wait(phil->meal_lock);
	phil->last_meal = get_time();
	phil->meals_eaten++;
	sem_post(phil->meal_lock);
	print_status(data, phil->id, EATING);
	usleep(data->time_to_eat * 1000);
	sem_post(data->table.forks);
	sem_post(data->table.forks);
}

static void	phil_sleep(t_phil *phil)
{
	t_data	*data;

	data = (t_data *)phil->data;
	print_status(data, phil->id, SLEEPING);
	usleep(data->time_to_sleep * 1000);
}

static void	phil_think(t_phil *phil)
{
	t_data	*data;

	data = (t_data *)phil->data;
	print_status(data, phil->id, THINKING);
	usleep(100);
}

void	*phil_life(void *arg)
{
	t_phil	*phil;

	phil = (t_phil *)arg;
	if (phil->id % 2 == 0)
		usleep(100);
	phil->last_meal = get_time();
	pthread_create(&phil->watcher, NULL, awareness, phil);
	while (true)
	{
		phil_eat(phil);
		phil_sleep(phil);
		phil_think(phil);
	}
	return (NULL);
}
