/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 17:59:04 by amagno-r          #+#    #+#             */
/*   Updated: 2025/07/31 19:12:06 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void    phil_eat(t_data *data, t_phil *phil)
{
    if (phil->left_fork < phil->right_fork)
    {
        pthread_mutex_lock(&data->table.forks[phil->left_fork]);
        print_status(data, phil->id, FORK);
        pthread_mutex_lock(&data->table.forks[phil->right_fork]);
        print_status(data, phil->id, FORK);
    }
    else
    {
        pthread_mutex_lock(&data->table.forks[phil->right_fork]);
        print_status(data, phil->id, FORK);
        pthread_mutex_lock(&data->table.forks[phil->left_fork]);
        print_status(data, phil->id, FORK);
    }
    pthread_mutex_lock(&phil->eat_lock);
    phil->last_meal = get_time();
    phil->meals_eaten++;
    pthread_mutex_unlock(&phil->eat_lock);
    print_status(data, phil->id, EATING);
    usleep(data->time_to_eat * 1000);
    pthread_mutex_unlock(&data->table.forks[phil->left_fork]);
    pthread_mutex_unlock(&data->table.forks[phil->right_fork]);
}
static void    phil_sleep(t_data *data, t_phil *phil)
{
    print_status(data, phil->id, SLEEPING);
    usleep(data->time_to_sleep * 1000);
}

static void    phil_think(t_data *data, t_phil *phil)
{
    print_status(data, phil->id, THINKING);
    usleep(100);
}

void    *phil_life(void *arg)
{
    t_phil *phil = (t_phil *)arg;
    t_data *data = (t_data *)phil->data;
    
    if (phil->id % 2 == 0)
        usleep(100);
    while (!is_simulation_over(data))
    {
        phil_eat(data, phil);
        if (is_simulation_over(data))
            break;
        phil_sleep(data, phil);
        if (is_simulation_over(data))
            break;
        phil_think(data, phil);
    }
    return (NULL);
}