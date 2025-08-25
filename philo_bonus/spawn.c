/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 21:30:00 by amagno-r          #+#    #+#             */
/*   Updated: 2025/08/25 23:01:51 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <sys/wait.h>
#include <unistd.h>

bool	init_table(t_data *data, int phil_count)
{
	data->table.phil_count = phil_count;
	data->table.phil = calloc(data->table.phil_count, sizeof(t_phil));
	if(!open_program_sems(data))
        return (false);
	if(!open_simulation_sems(&data->table))
        return (false);
	if(!open_fork_sems(&data->table))
        return (false);
    return (true);
}
bool spawn(t_data *data)
{
    int i;

    i = 0;
    while (i < data->table.phil_count)
    {
        data->table.phil[i].id = i + 1;
        data->table.phil[i].meals_eaten = 0;
        data->table.phil[i].data = data;
        open_phil_sems(&data->table.phil[i]);
        data->table.phil[i].pid = fork();
        if (data->table.phil[i].pid == 0)
        {
            phil_life(&data->table.phil[i]);
        } else if (data->table.phil[i].pid < 0)
            return (false);
        i++;
    }
    return (true);
}