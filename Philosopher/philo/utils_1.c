/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:51:25 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/12 17:53:00 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	time_ms(struct timeval *tv)
{
	return ((long long)(tv->tv_sec * 1000) + (tv->tv_usec / 1000));
}

void	kill_philos(t_philo *philos, int n_of_philos)
{
	int	i;

	i = 0;
	while (i < n_of_philos)
	{
		pthread_mutex_lock(&philos[i].alive_protection);
		philos[i].alive = 0;
		pthread_mutex_unlock(&philos[i].alive_protection);
		i++;
	}
}

void	destroy_mutex(t_philo *philos, int n_of_philos)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&philos[0].stuff->lock);
	while (i < n_of_philos)
	{
		pthread_mutex_destroy(&philos[i].eat_protection);
		pthread_mutex_destroy(&philos[i].alive_protection);
		pthread_mutex_destroy(&philos[i].time_protection);
		pthread_mutex_destroy(&philos[i].stuff->forks[i]);
		i++;
	}
}

bool	is_alive(t_philo *philo)
{
	bool			alive;

	pthread_mutex_lock(&philo->alive_protection);
	alive = philo->alive;
	pthread_mutex_unlock(&philo->alive_protection);
	return (alive);
}

int	ft_abs(int x)
{
	if (x < 0)
		return (-x);
	return (x);
}
