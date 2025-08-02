/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:20:51 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/31 17:02:12 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	run_philos(t_stuff *stuff)
{
	int	i;

	i = 0;
	while (i < stuff->number_of_philos)
	{
		stuff->philo_id = i + 1;
		stuff->philos[i] = fork();
		if (stuff->philos[i] == 0)
		{
			run_simulation(stuff);
		}
		if (stuff->philos[i] == -1)
		{
			kill_philos(stuff, i);
			clean_up(stuff);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	gettimeofday(&stuff->tv_start, NULL);
	sem_post(stuff->lock);
	wait_child(stuff);
}

void	allocate_philos_forks(t_stuff *stuff)
{
	stuff->philos = malloc (sizeof(pid_t) * stuff->number_of_philos);
	if (!stuff->philos)
	{
		exit(EXIT_FAILURE);
	}
	sem_unlink("/forks");
	sem_unlink("/lock");
	stuff->lock = sem_open("/lock", O_CREAT, 0777, 0);
	stuff->forks = sem_open("/forks", O_CREAT, 0777, stuff->number_of_philos);
	if (!stuff->forks || !stuff->lock)
	{
		clean_up(stuff);
		exit(EXIT_FAILURE);
	}
}

void	init_philos(t_stuff *stuff)
{
	int	t_to_eat_sleep;

	stuff->t_to_think = ft_abs(stuff->t_to_eat - stuff->t_to_sleep) + 10;
	t_to_eat_sleep = stuff->t_to_eat + stuff->t_to_sleep;
	if (t_to_eat_sleep < stuff->t_to_die)
	{
		while (stuff->t_to_think + t_to_eat_sleep >= stuff->t_to_die)
			stuff->t_to_think /= 2;
	}
	allocate_philos_forks(stuff);
	run_philos(stuff);
}
