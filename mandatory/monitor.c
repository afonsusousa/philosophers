/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:16 by amagno-r          #+#    #+#             */
/*   Updated: 2025/09/17 21:47:25 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_simulation_over(t_data *data)
{
	int	ret;

	pthread_mutex_lock(&data->write_lock);
	ret = data->simulation_end;
	pthread_mutex_unlock(&data->write_lock);
	return (ret);
}

static bool	check_bellies(t_data *data, t_phil *phil)
{
	if (get_time() - phil->last_meal > data->time_to_die
		&& phil->meals_eaten != data->must_eat)
	{
		pthread_mutex_lock(&data->write_lock);
		if (!data->simulation_end)
		{
			data->simulation_end = 1;
			printf("%ld %d died\n", get_simtime(data), phil->id);
		}
		pthread_mutex_unlock(&data->write_lock);
		return (false);
	}
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;
	int		finished_eating;

	data = (t_data *)arg;
	while (1)
	{
		finished_eating = 0;
		i = -1;
		while (++i < data->table.phil_count)
		{
			pthread_mutex_lock(&data->table.phil[i].eat_lock);
			if (data->must_eat != -1
				&& data->table.phil[i].meals_eaten >= data->must_eat)
				finished_eating++;
			if (!check_bellies(data, &data->table.phil[i]))
			{
				pthread_mutex_unlock(&data->table.phil[i].eat_lock);
				return (NULL);
			}
			pthread_mutex_unlock(&data->table.phil[i].eat_lock);
		}
		if (data->must_eat != -1 && finished_eating == data->table.phil_count)
		{
			pthread_mutex_lock(&data->write_lock);
			data->simulation_end = 1;
			pthread_mutex_unlock(&data->write_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
