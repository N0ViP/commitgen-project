/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:21:23 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/31 17:01:05 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	thinking(t_stuff *stuff)
{
	if (!is_alive(stuff))
		return ;
	print(stuff, "is thinking\n");
	usleep(stuff->t_to_think * 1000);
}

void	sleeping(t_stuff *stuff)
{
	if (!is_alive(stuff))
		return ;
	print(stuff, "is sleeping\n");
	usleep(stuff->t_to_sleep * 1000);
}

void	eating(t_stuff *stuff)
{
	take_forks(stuff);
	if (!is_alive(stuff))
	{
		put_forks(stuff);
		return ;
	}
	sem_wait(stuff->time_protection);
	gettimeofday(&stuff->tv_beg, NULL);
	sem_post(stuff->time_protection);
	print(stuff, "is eating\n");
	usleep(stuff->t_to_eat * 1000);
	put_forks(stuff);
	sem_wait(stuff->eat_protection);
	stuff->n_eat++;
	sem_post(stuff->eat_protection);
}

void	*start(void *arg)
{
	t_stuff	*stuff;

	stuff = (t_stuff *) arg;
	if (!(stuff->philo_id % 2))
		usleep(stuff->t_to_eat * 1000);
	while (is_alive(stuff))
	{
		thinking(stuff);
		eating(stuff);
		sleeping(stuff);
	}
	return (NULL);
}

void	run_simulation(t_stuff *stuff)
{
	pthread_t	philo;

	init_semaphores(stuff);
	sem_wait(stuff->lock);
	sem_post(stuff->lock);
	gettimeofday(&stuff->tv_start, NULL);
	stuff->tv_beg = stuff->tv_start;
	if (pthread_create(&philo, NULL, start, stuff))
	{
		clean_sems(stuff);
		exit(EXIT_FAILURE);
	}
	monitor(stuff, philo);
}
