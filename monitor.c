/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:16 by amagno-r          #+#    #+#             */
/*   Updated: 2025/07/31 19:19:28 by amagno-r         ###   ########.fr       */
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

static int	check_bellies(t_data *data, int i, int *full_bellies)
{
	if (data->table.phil[i].meals_eaten == data->must_eat)
		(*full_bellies)++;
	if (*full_bellies == data->table.phil_count)
	{
		pthread_mutex_lock(&data->end_lock);
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->end_lock);
		return (0);
	}
	if (get_time() - data->table.phil[i].last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->end_lock);
		data->simulation_end = 1;
		pthread_mutex_unlock(&data->end_lock);
		print_status(data, data->table.phil[i].id, DEATH);
		return (0);
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	int		full_bellies;

	data = (t_data *)arg;
	while (!is_simulation_over(data))
	{
		i = 0;
		full_bellies = 0;
		while (i < data->table.phil_count)
		{
			pthread_mutex_lock(&data->table.phil[i].eat_lock);
			if (!check_bellies(data, i, &full_bellies))
			{
				pthread_mutex_unlock(&data->table.phil[i].eat_lock);
				break ;
			}
			pthread_mutex_unlock(&data->table.phil[i].eat_lock);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
