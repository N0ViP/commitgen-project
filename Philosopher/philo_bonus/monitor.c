/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:03:42 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 16:20:58 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	jon_philo(t_stuff *stuff, pthread_t philo)
{
	sem_wait(stuff->alive_protection);
	stuff->alive = false;
	sem_post(stuff->alive_protection);
	pthread_join(philo, NULL);
	clean_sems(stuff);
}

void	check_alive(t_stuff *stuff, pthread_t philo)
{
	struct timeval	tv;
	long long		time;

	sem_wait(stuff->time_protection);
	time = time_ms(&stuff->tv_beg);
	sem_post(stuff->time_protection);
	gettimeofday(&tv, NULL);
	if (time_ms(&tv) - time >= stuff->t_to_die)
	{
		jon_philo(stuff, philo);
		exit(EXIT_FAILURE);
	}
}

void	check_eat(t_stuff *stuff, pthread_t philo)
{
	int	n;

	sem_wait(stuff->eat_protection);
	n = stuff->n_eat;
	sem_post(stuff->eat_protection);
	if (stuff->must_eat && n >= stuff->must_eat)
	{
		jon_philo(stuff, philo);
		exit(EXIT_SUCCESS);
	}
}

void	monitor(t_stuff *stuff, pthread_t philo)
{
	while (true)
	{
		check_alive(stuff, philo);
		check_eat(stuff, philo);
	}
}
