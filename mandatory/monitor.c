/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:16 by amagno-r          #+#    #+#             */
/*   Updated: 2025/09/16 18:39:35 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_simulation_over(t_data *data)
{
	pthread_mutex_lock(&data->end_lock);
	if (data->simulation_end)
	{
		pthread_mutex_unlock(&data->end_lock);
		return (1);
	}
	pthread_mutex_unlock(&data->end_lock);
	return (0);
}

static bool	check_bellies(t_data *data, t_phil *phil)
{
	pthread_mutex_lock(&data->end_lock);
	if (data->table.meal_watcher == data->table.phil_count)
	{
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->end_lock);
		return (false);
	}
	pthread_mutex_unlock(&data->end_lock);
	pthread_mutex_lock(&data->end_lock);
	if (get_time() - phil->last_meal > data->time_to_die
		&& phil->meals_eaten != data->must_eat)
	{
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->end_lock);
		print_status(data, phil->id, DEATH);
		return (false);
	}
	pthread_mutex_unlock(&data->end_lock);
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!is_simulation_over(data))
	{
		i = -1;
		while (++i < data->table.phil_count)
		{
			pthread_mutex_lock(&data->table.phil[i].eat_lock);
			if (!check_bellies(data, &data->table.phil[i]))
			{
				pthread_mutex_unlock(&data->table.phil[i].eat_lock);
				break ;
			}
			pthread_mutex_unlock(&data->table.phil[i].eat_lock);
		}
		usleep(1000);
	}
	return (NULL);
}
