/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:59:04 by amagno-r          #+#    #+#             */
/*   Updated: 2025/09/16 19:09:49 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdbool.h>

static bool take_forks(t_data *data, t_phil *phil)
{
	if (phil->left_fork < phil->right_fork)
	{
		pthread_mutex_lock(&data->table.forks[phil->left_fork]);
		print_status(data, phil->id, FORK);
		if (phil->left_fork == phil->right_fork)
		{
			pthread_mutex_unlock(&data->table.forks[phil->left_fork]);
			return (false);
		}
		pthread_mutex_lock(&data->table.forks[phil->right_fork]);
		print_status(data, phil->id, FORK);
	}
	else
	{
		pthread_mutex_lock(&data->table.forks[phil->right_fork]);
		print_status(data, phil->id, FORK);
		if (phil->left_fork == phil->right_fork)
		{
			pthread_mutex_unlock(&data->table.forks[phil->right_fork]);
			return (false);
		}
		pthread_mutex_lock(&data->table.forks[phil->left_fork]);
		print_status(data, phil->id, FORK);
	}
	return (true);
}

static bool	phil_eat(t_data *data, t_phil *phil)
{
	if (phil->meals_eaten == data->must_eat)
		return (false);
	if (!take_forks(data, phil))
		return (false);
	pthread_mutex_lock(&phil->eat_lock);
	phil->last_meal = get_time();
	phil->meals_eaten++;
	pthread_mutex_unlock(&phil->eat_lock);
	print_status(data, phil->id, EATING);
	usleep(data->time_to_eat * 1000);
	pthread_mutex_unlock(&data->table.forks[phil->left_fork]);
	pthread_mutex_unlock(&data->table.forks[phil->right_fork]);
	pthread_mutex_lock(&data->end_lock);
	if (phil->meals_eaten == data->must_eat)
		data->table.meal_watcher++;
	pthread_mutex_unlock(&data->end_lock);
	return (true);
}

static void	phil_sleep_think(t_data *data, t_phil *phil)
{
	if (is_simulation_over(data))
		return ;
	print_status(data, phil->id, SLEEPING);
	usleep(data->time_to_sleep * 1000);
	if (is_simulation_over(data))
		return ;
	print_status(data, phil->id, THINKING);
	usleep(100);
}

void	*phil_life(void *arg)
{
	t_phil	*phil;
	t_data	*data;

	phil = (t_phil *)arg;
	data = (t_data *)phil->data;
	if (phil->id % 2 == 0)
		usleep(100);
	while (!is_simulation_over(data) && phil_eat(data, phil))
		phil_sleep_think(data, phil);
	return (NULL);
}
