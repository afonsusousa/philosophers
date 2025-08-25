/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 22:42:29 by amagno-r          #+#    #+#             */
/*   Updated: 2025/08/25 23:03:15 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <signal.h>

static void	clean_phils(t_data *data)
{
	int	i;

	i = 0;
	if (data->table.phil)
	{
		while (i < data->table.phil_count)
		{
			if (data->table.phil[i].meal_lock)
			{
				sem_close(data->table.phil[i].meal_lock);
				if (data->table.phil[i].meal_lock_name)
				{
					sem_unlink(data->table.phil[i].meal_lock_name);
					free(data->table.phil[i].meal_lock_name);
					data->table.phil[i].meal_lock_name = NULL;
				}
				data->table.phil[i].meal_lock = NULL;
			}
			i++;
		}
		free(data->table.phil);
		data->table.phil = NULL;
	}
}

void	cleanup(t_data *data)
{
	if (data->write_lock)
	{
		sem_close(data->write_lock);
		sem_unlink("WRITE_LOCK");
	}
	if (data->table.death)
	{
		sem_close(data->table.death);
		sem_unlink("DEATH_FLAG");
	}
	if (data->table.full)
	{
		sem_close(data->table.full);
		sem_unlink("FULL_COUNT");
	}
	sem_close(data->table.forks);
	sem_unlink("FORK_SEM");
	clean_phils(data);
}

void	cleanup_dead_child(t_phil *phil)
{
	if (phil->meal_lock)
	{
		sem_close(phil->meal_lock);
		if (phil->meal_lock_name)
		{
			sem_unlink(phil->meal_lock_name);
			free(phil->meal_lock_name);
			phil->meal_lock_name = NULL;
		}
		phil->meal_lock = NULL;
	}
}

void	kill_all_children(t_data *data)
{
	static bool	already_killed = false;
	int			i;

	if (already_killed)
		return ;
	already_killed = true;
	i = 0;
	while (i < data->table.phil_count)
	{
		if (data->table.phil[i].pid > 0)
		{
			kill(data->table.phil[i].pid, SIGKILL);
			waitpid(data->table.phil[i].pid, NULL, 0);
			cleanup_dead_child(&data->table.phil[i]);
			data->table.phil[i].pid = 0;
		}
		i++;
	}
}
