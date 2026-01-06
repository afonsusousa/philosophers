/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:01:38 by amagno-r          #+#    #+#             */
/*   Updated: 2025/09/16 16:48:26 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdbool.h>

typedef enum e_action
{
	FORK,
	EATING,
	THINKING,
	SLEEPING,
	DEATH
}					t_action;

typedef struct s_phil
{
	int				id;
	int				left_fork;
	int				right_fork;
	pthread_mutex_t	eat_lock;
	bool 			is_full;
	long			last_meal;
	pthread_t		thread;
	void			*data;
	int				meals_eaten;
}					t_phil;

typedef struct s_table
{
	pthread_mutex_t	*forks;
	int				phil_count;
	t_phil			*phil;
	int				meal_watcher;
}					t_table;

typedef struct s_data
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	t_table			table;
	long			sim_start;
	pthread_mutex_t	write_lock;
	int				simulation_end;
}					t_data;

int					ft_atoi(const char *nptr);
long				get_time(void);
long				get_simtime(t_data *data);
void				print_status(t_data *data, int id, t_action action);

void				*phil_life(void *arg);

int					is_simulation_over(t_data *data);
void				*monitor_routine(void *arg);

int					init_philosophers(t_data *data);
int					init_mutexes(t_data *data);
int					start_simulation(t_data *data);
void				cleanup(t_data *data);

#endif