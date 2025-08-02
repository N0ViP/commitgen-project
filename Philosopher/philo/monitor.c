/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 21:47:04 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/31 16:55:24 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_philos(t_philo *philos, int i)
{
	struct timeval	tv;
	long long		time;
	int				t_to_die;

	t_to_die = philos[i].stuff->t_to_die;
	pthread_mutex_lock(&philos[i].time_protection);
	time = time_ms(&philos[i].tv_beg);
	pthread_mutex_unlock(&philos[i].time_protection);
	gettimeofday(&tv, NULL);
	if (time && time_ms(&tv) - time >= t_to_die)
	{
		kill_philos(philos, philos[i].stuff->number_of_philos);
		print(&philos[i], "died\n");
		join_philos(philos, philos[i].stuff->number_of_philos);
		return (false);
	}
	return (true);
}

bool	monitoring_1(t_philo *philos)
{
	int	i;
	int	n;

	n = philos[0].stuff->number_of_philos;
	while (true)
	{
		i = 0;
		while (i < n)
		{
			if (!check_philos(philos, i))
				return (false);
			i++;
		}
	}
	return (true);
}

bool	monitoring_2(t_philo *philos)
{
	int	i;
	int	cnt;
	int	n;

	n = philos[0].stuff->number_of_philos;
	while (true)
	{
		i = 0;
		cnt = 0;
		while (i < n)
		{
			if (!check_philos(philos, i))
				return (false);
			pthread_mutex_lock(&philos[i].eat_protection);
			if (philos[i].eat >= philos[0].stuff->must_eat)
				cnt++;
			pthread_mutex_unlock(&philos[i].eat_protection);
			i++;
		}
		if (cnt == n)
			return (kill_philos(philos, n), join_philos(philos, n), true);
	}
	return (true);
}

void	*monitoring(void *arg)
{
	t_philo		*philos;
	long long	reval;

	philos = (t_philo *) arg;
	if (!philos->stuff->must_eat)
		reval = monitoring_1(philos);
	else
		reval = monitoring_2(philos);
	return ((void *)reval);
}
