/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:55:12 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/12 17:50:29 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print(t_philo *philo, char *str)
{
	struct timeval	tv;

	pthread_mutex_lock(&philo->stuff->lock);
	gettimeofday(&tv, NULL);
	printf("%lld\t%d\t%s", time_ms(&tv) - \
		time_ms(&philo->stuff->tv_start), philo->first_fork + 1, str);
	pthread_mutex_unlock(&philo->stuff->lock);
}

void	take_fork(t_philo *philo, int fork)
{
	pthread_mutex_lock(&philo->stuff->forks[fork]);
	if (!is_alive(philo))
		return ;
	print(philo, "has taken a fork\n");
}

void	join_philos(t_philo *philos, int n_of_philos)
{
	int	i;

	i = 0;
	while (i < n_of_philos)
	{
		pthread_join(philos[i].stuff->philos[i], NULL);
		i++;
	}
	destroy_mutex(philos, philos->stuff->number_of_philos);
}

void	put_fork(t_philo *philo, int fork)
{
	pthread_mutex_unlock(&philo->stuff->forks[fork]);
}

void	take_forks(t_philo *philo)
{
	if (philo->first_fork % 2)
	{
		take_fork(philo, philo->first_fork);
		take_fork(philo, philo->second_fork);
	}
	else
	{
		take_fork(philo, philo->second_fork);
		take_fork(philo, philo->first_fork);
	}
}
