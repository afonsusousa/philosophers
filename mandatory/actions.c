/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:59:04 by amagno-r          #+#    #+#             */
/*   Updated: 2025/09/17 23:29:39 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <stdbool.h>

static bool	take_forks(t_data *data, t_phil *phil)
{
	int	first;
	int	second;

	first = phil->left_fork;
	second = phil->right_fork;
	if (phil->id % 2)
	{
		first = phil->right_fork;
		second = phil->left_fork;
	}
	pthread_mutex_lock(&data->table.forks[first]);
	print_status(data, phil->id, FORK);
	if (first == second)
		return (pthread_mutex_unlock(&data->table.forks[first]) && false);
	pthread_mutex_lock(&data->table.forks[second]);
	print_status(data, phil->id, FORK);
	return (true);
}

static bool	release_forks(t_data *data, t_phil *phil)
{
	int	first;
	int	second;

	first = phil->left_fork;
	second = phil->right_fork;
	if (phil->id % 2)
	{
		first = phil->right_fork;
		second = phil->left_fork;
	}
	pthread_mutex_unlock(&data->table.forks[first]);
	pthread_mutex_unlock(&data->table.forks[second]);
	return (true);
}

void	*phil_life(void *arg)
{
	t_phil	*phil;
	t_data	*data;

	phil = (t_phil *)arg;
	data = (t_data *)phil->data;
	while (true)
	{
		if (is_simulation_over(data) || !take_forks(data, phil))
			break ;
		pthread_mutex_lock(&phil->eat_lock);
		phil->last_meal = get_time();
		phil->meals_eaten++;
		pthread_mutex_unlock(&phil->eat_lock);
		print_status(data, phil->id, EATING);
		usleep(data->time_to_eat * 1000);
		release_forks(data, phil);
		print_status(data, phil->id, SLEEPING);
		usleep(data->time_to_sleep * 1000);
		print_status(data, phil->id, THINKING);
		usleep(1000);
	}
	return (NULL);
}
