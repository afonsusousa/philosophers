/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 18:01:22 by amagno-r          #+#    #+#             */
/*   Updated: 2025/08/31 17:33:07 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	result;

	sign = 1;
	result = 0;
	while (*nptr == ' ' || (*nptr >= 9 && *nptr <= 13))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
		if (*nptr++ == '-')
			sign *= -1;
	while (*nptr >= '0' && *nptr <= '9')
		result = result * 10 + (*nptr++ - '0');
	if (*nptr != '\0')
		return (-1);
	return (result * sign);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_simtime(t_data *data)
{
	return (get_time() - data->sim_start);
}

void	print_status(t_data *data, int id, t_action action)
{
	long	t_stamp;

	t_stamp = get_simtime(data);
	if (action != DEATH)
	{
		pthread_mutex_lock(&data->end_lock);
		if (data->simulation_end)
		{
			pthread_mutex_unlock(&data->end_lock);
			return ;
		}
		pthread_mutex_unlock(&data->end_lock);
	}
	pthread_mutex_lock(&data->write_lock);
	if (action == EATING)
		printf("%ld %d is eating\n", t_stamp, id);
	else if (action == THINKING)
		printf("%ld %d is thinking\n", t_stamp, id);
	else if (action == SLEEPING)
		printf("%ld %d is sleeping\n", t_stamp, id);
	else if (action == FORK)
		printf("%ld %d has taken a fork\n", t_stamp, id);
	else if (action == DEATH)
		printf("%ld %d died\n", t_stamp, id);
	pthread_mutex_unlock(&data->write_lock);
}
