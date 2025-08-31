/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 15:38:16 by amagno-r          #+#    #+#             */
/*   Updated: 2025/07/31 19:25:45 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	if (!init_mutexes(&data) || !init_philosophers(&data))
	{
		printf("Error: Initialization failed\n");
		return (cleanup(&data), 1);
	}
	if (!start_simulation(&data))
	{
		printf("Error: Failed to start simulation\n");
		return (cleanup(&data), 1);
	}
	return (cleanup(&data), 0);
}
