/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:53:36 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/31 16:57:24 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	thinking(t_philo *philo)
{
	if (!is_alive(philo))
		return ;
	print(philo, "is thinking\n");
	usleep(philo->stuff->t_to_think * 1000);
}

void	sleeping(t_philo *philo)
{
	if (!is_alive(philo))
		return ;
	print(philo, "is sleeping\n");
	usleep(philo->stuff->t_to_sleep * 1000);
}

void	eating(t_philo *philo)
{
	take_forks(philo);
	if (!is_alive(philo))
	{
		put_fork(philo, philo->first_fork);
		put_fork(philo, philo->second_fork);
		return ;
	}
	pthread_mutex_lock(&philo->time_protection);
	gettimeofday(&philo->tv_beg, NULL);
	pthread_mutex_unlock(&philo->time_protection);
	print(philo, "is eating\n");
	usleep(philo->stuff->t_to_eat * 1000);
	put_fork(philo, philo->first_fork);
	put_fork(philo, philo->second_fork);
	pthread_mutex_lock(&philo->eat_protection);
	philo->eat++;
	pthread_mutex_unlock(&philo->eat_protection);
}

void	*run_simulation(void *arg)
{
	t_philo			*philo;

	philo = (t_philo *) arg;
	if (!(philo->first_fork % 2))
		usleep(philo->stuff->t_to_eat * 1000);
	pthread_mutex_lock(&philo->stuff->lock);
	pthread_mutex_unlock(&philo->stuff->lock);
	pthread_mutex_lock(&philo->time_protection);
	philo->tv_beg = philo->stuff->tv_start;
	pthread_mutex_unlock(&philo->time_protection);
	while (is_alive(philo))
	{
		thinking(philo);
		eating(philo);
		sleeping(philo);
	}
	return (NULL);
}
