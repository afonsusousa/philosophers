/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:21:53 by amagno-r          #+#    #+#             */
/*   Updated: 2025/08/25 22:53:07 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
#define PHILOSOPHERS_H

#include <semaphore.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <fcntl.h>

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
	pid_t			pid;
	char			*meal_lock_name;
	sem_t       	*meal_lock;
	long			last_meal;
	pthread_t		watcher;
	void			*data;
	int				meals_eaten;
}					t_phil;

typedef struct s_table
{
	sem_t	        *forks;
	int				phil_count;
	t_phil			*phil;
	sem_t			*full;
	sem_t			*death;
}					t_table;

typedef struct s_data
{
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	t_table			table;
	long			sim_start;
	sem_t	        *write_lock;
	pthread_t		death_watcher;
	pthread_t		meal_watcher;
}					t_data;

// Function prototypes

// utils.c
int		ft_atoi(const char *nptr);
long	get_time(void);
long	get_simtime(t_data *data);
void	print_status(t_data *data, int id, t_action action);
bool	init_table(t_data *data, int phil_count);

// sems.c
char	*get_sem_name(t_phil *phil, char *sem_type);
void	open_phil_sems(t_phil *phil);
bool	open_program_sems(t_data *data);
bool	open_simulation_sems(t_table *table);
bool	open_fork_sems(t_table *table);

// spawn.c
bool	spawn(t_data *data);

// actions.c
void	*phil_life(void *arg);

// monitor.c
void	die(t_data *data, t_phil *phil);
void	*awareness(void *arg);
void	*meal_watcher(void *arg);
void	*death_checker(void *arg);
void	kill_all_children(t_data *data);

// cleanup.c
void	cleanup(t_data *data);
void	cleanup_dead_child(t_phil *phil);

// philosophers.c
void	init_data(t_data *data, int argc, char **argv);

#endif