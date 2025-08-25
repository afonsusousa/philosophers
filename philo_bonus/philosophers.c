/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 21:10:25 by amagno-r          #+#    #+#             */
/*   Updated: 2025/08/25 22:42:45 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philosophers.h"
#include <sys/wait.h>

void	init_data(t_data *data, int argc, char **argv)
{
	data->table.phil_count = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat = ft_atoi(argv[5]);
	else
		data->must_eat = -1;
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 5 && argc != 6)
	{
		printf("Wrong number of arguments\n");
		return (1);
	}
	init_data(&data, argc, argv);
	if (data.table.phil_count <= 0 || data.time_to_die <= 0
		|| data.time_to_eat <= 0 || data.time_to_sleep <= 0)
	{
		printf("Error: All arguments must be positive integers\n");
		return (1);
	}
	data.sim_start = get_time();
	if (!init_table(&data, data.table.phil_count)){
		return (cleanup(&data), 1);
	}
	spawn(&data);
	if (pthread_create(&data.death_watcher, NULL, death_checker, &data) != 0) {
		printf("Error: Failed to create death watcher thread\n");
		return (cleanup(&data), 1);
	}
	if (data.must_eat > 0) {
		if (pthread_create(&data.meal_watcher, NULL, meal_watcher, &data) != 0) {
			return (cleanup(&data), 1);
		}
	}
	pthread_join(data.death_watcher, NULL);
	if (data.must_eat > 0) {
		pthread_join(data.meal_watcher, NULL);
	}
	return (cleanup(&data), 0);
}