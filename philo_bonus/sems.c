/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sems.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amagno-r <amagno-r@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 20:20:22 by amagno-r          #+#    #+#             */
/*   Updated: 2025/08/25 23:01:27 by amagno-r         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#include <fcntl.h>
#include <sys/stat.h>

char *get_sem_name(t_phil *phil, char *sem_type)
{
    int id;
    int digits;
    char *ret;
    char id_str[12];
    digits = 1;
    id = phil->id;
    while (id >= 10)
    {
        id /= 10;
        digits++;
    }
    ret = calloc(5 + digits + strlen(sem_type) + 1, 1);
    if (!ret)
        return (NULL);
    sprintf(id_str, "%d", phil->id);
    strcat(ret, "PHIL_");
    strcat(ret, id_str);
    strcat(ret, sem_type);
    return (ret);
}

void open_phil_sems(t_phil *phil)
{
    phil->meal_lock_name = get_sem_name(phil, "MEAL");
    if (phil->meal_lock_name == NULL)
    {
        perror("Failed to generate semaphore name");
        exit(1);
    }
    sem_unlink(phil->meal_lock_name);
    phil->meal_lock = sem_open(phil->meal_lock_name, O_CREAT | O_EXCL,
                               S_IRUSR | S_IWUSR, 1);
    if (phil->meal_lock == SEM_FAILED)
    {
        perror("Failed to create meal semaphore");
        free(phil->meal_lock_name);
        exit(1);
    }
}
bool open_program_sems(t_data *data)
{
    sem_unlink("WRITE_LOCK");
    sem_unlink("END_LOCK");
    data->write_lock = sem_open("WRITE_LOCK", O_CREAT | O_EXCL,
                                S_IRUSR | S_IWUSR, 1);
    if (data->write_lock == SEM_FAILED)
    {
        perror("Failed to create write lock semaphore");
        return (false);
    }
    return (true);
}

bool open_simulation_sems(t_table *table)
{
    sem_unlink("FULL_COUNT");
    sem_unlink("DEATH_FLAG");
    table->full = sem_open("FULL_COUNT", O_CREAT | O_EXCL,
                           S_IRUSR | S_IWUSR, 0);
    if (table->full == SEM_FAILED)
    {
        perror("Failed to create full count semaphore");
        return (false);
    }
    table->death = sem_open("DEATH_FLAG", O_CREAT | O_EXCL,
                            S_IRUSR | S_IWUSR, 0);
    if (table->death == SEM_FAILED)
    {
        perror("Failed to create death flag semaphore");
        sem_close(table->full);
        sem_unlink("FULL_COUNT");
        return (false);
    }
    return (true);
}

bool open_fork_sems(t_table *table)
{
    sem_unlink("FORK_SEM");
    table->forks = sem_open("FORK_SEM", O_CREAT | O_EXCL,
                            S_IRUSR | S_IWUSR, table->phil_count);
    if (table->forks == SEM_FAILED)
    {
        perror("Failed to create fork semaphore");
        return (false);
    }
    return (true);
}