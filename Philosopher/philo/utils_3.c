/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 21:24:18 by yjaafar           #+#    #+#             */
/*   Updated: 2025/06/29 21:24:23 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(char *s)
{
	long	res;

	if (!*s)
		return (-1);
	res = 0;
	while (*s >= 48 && *s <= 57)
	{
		res = (res << 3) + (res << 1) + (*s++ & 0X0f);
		if (res > INT_MAX)
			return (-1);
	}
	if (*s)
		return (-1);
	return ((int) res);
}

bool	init_each_philo(t_philo *philo, t_stuff *stuff, int i)
{
	pthread_mutex_t	*mtx[4];

	philo->stuff = stuff;
	philo->alive = 1;
	philo->eat = 0;
	philo->first_fork = (i);
	philo->second_fork = (i + 1) % (stuff->number_of_philos);
	philo->tv_beg = (struct timeval){0};
	mtx[0] = &stuff->forks[i];
	mtx[1] = &philo->eat_protection;
	mtx[2] = &philo->time_protection;
	mtx[3] = &philo->alive_protection;
	return (init_mutex(mtx));
}

bool	one_philo(t_stuff *stuff)
{
	pthread_mutex_t	fork;

	if (pthread_mutex_init(&fork, NULL))
		return (false);
	printf("0\t1\tis thinking\n");
	pthread_mutex_lock(&fork);
	printf("0\t1\thas taken a fork\n");
	usleep(1000 * stuff->t_to_die);
	pthread_mutex_unlock(&fork);
	printf("%d\t1\tdied\n", stuff->t_to_die);
	return (true);
}

bool	init_mutex(pthread_mutex_t *mtx[4])
{
	int	i;

	i = 0;
	while (i < 4)
	{
		if (pthread_mutex_init(mtx[i], NULL))
		{
			while (--i <= 0)
			{
				pthread_mutex_destroy(mtx[i]);
			}
			return (false);
		}
		i++;
	}
	return (true);
}

bool	init_philo(t_philo *philos, t_stuff *stuff)
{
	int		i;

	i = 0;
	while (i < stuff->number_of_philos)
	{
		if (!init_each_philo(&philos[i], stuff, i))
		{
			pthread_mutex_unlock(&stuff->lock);
			destroy_mutex(philos, i);
			free(philos);
			free(stuff->philos);
			free(stuff->forks);
			return (false);
		}
		i++;
	}
	return (true);
}
