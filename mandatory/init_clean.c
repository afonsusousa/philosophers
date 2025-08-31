/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_clean.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:00:52 by amagno-r          #+#    #+#             */
/*   Updated: 2025/07/31 19:27:58 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_philosophers(t_data *data)
{
	int	i;

	i = 0;
	data->table.phil = malloc(sizeof(t_phil) * data->table.phil_count);
	if (!data->table.phil)
		return (0);
	while (i < data->table.phil_count)
	{
		memset(&data->table.phil[i], 0, sizeof(t_phil));
		data->table.phil[i].id = i + 1;
		data->table.phil[i].left_fork = i;
		data->table.phil[i].right_fork = (i + 1) % data->table.phil_count;
		data->table.phil[i].data = data;
		data->table.phil[i].last_meal = get_time();
		if (pthread_mutex_init(&data->table.phil[i].eat_lock, NULL) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	init_mutexes(t_data *data)
{
	int	i;

	data->table.forks = malloc(sizeof(pthread_mutex_t)
			* data->table.phil_count);
	if (!data->table.forks)
		return (0);
	i = 0;
	while (i < data->table.phil_count)
	{
		if (pthread_mutex_init(&data->table.forks[i], NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->end_lock, NULL) != 0)
		return (0);
	return (1);
}

int	start_simulation(t_data *data)
{
	int			i;
	pthread_t	monitor_thread;

	data->sim_start = get_time();
	data->simulation_end = 0;
	if (pthread_create(&monitor_thread, NULL, monitor_routine, data) != 0)
		return (0);
	i = -1;
	while (++i < data->table.phil_count)
	{
		if (pthread_create(&data->table.phil[i].thread, NULL, phil_life,
				&data->table.phil[i]) != 0)
			return (0);
	}
	pthread_join(monitor_thread, NULL);
	i = -1;
	while (++i < data->table.phil_count)
	{
		pthread_join(data->table.phil[i].thread, NULL);
	}
	return (1);
}

void	cleanup(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->table.phil_count)
	{
		pthread_mutex_destroy(&data->table.forks[i]);
		pthread_mutex_destroy(&data->table.phil[i].eat_lock);
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->end_lock);
	free(data->table.forks);
	free(data->table.phil);
}
