/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/14 08:28:53 by yjaafar           #+#    #+#             */
/*   Updated: 2025/08/02 05:37:50 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	clean_up(t_stuff *stuff, t_philo *philos)
{
	free(stuff->philos);
	free(stuff->forks);
	free(philos);
}

static bool	allocate_stuff(t_stuff *stuff, t_philo **philos)
{
	stuff->philos = malloc(sizeof(pthread_t) * stuff->number_of_philos);
	stuff->forks = malloc(sizeof(pthread_mutex_t) * stuff->number_of_philos);
	*philos = malloc(sizeof(t_philo) * stuff->number_of_philos);
	if (!stuff->philos || !stuff->forks || !*philos)
	{
		clean_up(stuff, *philos);
		return (false);
	}
	if (pthread_mutex_init(&stuff->lock, NULL))
	{
		clean_up(stuff, *philos);
		return (false);
	}
	pthread_mutex_lock(&stuff->lock);
	return (true);
}

static bool	create_philos(t_philo *philos)
{
	t_stuff	*stuff;
	int		i;

	i = 0;
	stuff = philos->stuff;
	while (i < stuff->number_of_philos)
	{
		if (pthread_create(&stuff->philos[i], NULL, run_simulation, &philos[i]))
		{
			pthread_mutex_unlock(&stuff->lock);
			kill_philos(philos, i);
			join_philos(philos, i);
			clean_up(stuff, philos);
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	alloc_philos(t_stuff *stuff)
{
	t_philo	*philos;
	int		t_to_eat_sleep;
	bool	reval;

	stuff->t_to_think = ft_abs(stuff->t_to_eat - stuff->t_to_sleep) + 10;
	t_to_eat_sleep = stuff->t_to_eat + stuff->t_to_sleep;
	if (t_to_eat_sleep < stuff->t_to_die)
	{
		while (stuff->t_to_think + t_to_eat_sleep >= stuff->t_to_die)
			stuff->t_to_think /= 2;
	}
	if (!allocate_stuff(stuff, &philos))
		return (false);
	if (!init_philo(philos, stuff))
		return (false);
	if (!create_philos(philos))
		return (false);
	gettimeofday(&stuff->tv_start, NULL);
	pthread_mutex_unlock(&stuff->lock);
	reval = monitoring(philos);
	clean_up(stuff, philos);
	return (reval);
}

int	main(int ac, char *av[])
{
	t_stuff	stuff;
	int		reval;

	if (ac != 5 && ac != 6)
		return (write(2, "Invalid arguments!\n", 19), 1);
	stuff.must_eat = 0;
	stuff.number_of_philos = ft_atoi(av[1]);
	stuff.t_to_die = ft_atoi(av[2]);
	stuff.t_to_eat = ft_atoi(av[3]);
	stuff.t_to_sleep = ft_atoi(av[4]);
	if (stuff.number_of_philos <= 0
		|| stuff.t_to_die <= 0
		|| stuff.t_to_eat <= 0
		|| stuff.t_to_sleep <= 0)
		return (write(2, "Invalid arguments!\n", 19), 1);
	if (ac == 6)
	{
		stuff.must_eat = ft_atoi(av[5]);
		if (stuff.must_eat <= 0)
			return (write(2, "Invalid arguments!\n", 19), 1);
	}
	if (stuff.number_of_philos == 1)
		return (!one_philo(&stuff));
	reval = alloc_philos(&stuff);
	return (!reval);
}
