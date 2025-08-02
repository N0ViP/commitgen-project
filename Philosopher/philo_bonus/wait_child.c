/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:21:41 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 17:53:28 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	kill_philos(t_stuff *stuff, int n_of_philos)
{
	int	i;

	i = 0;
	while (i < n_of_philos)
	{
		kill(stuff->philos[i], SIGKILL);
		i++;
	}
}

void	philo_died(t_stuff *stuff, int pid)
{
	struct timeval	tv;
	int				i;

	i = 0;
	while (stuff->philos[i] != pid)
	{
		i++;
	}
	i++;
	kill_philos(stuff, stuff->number_of_philos);
	gettimeofday(&tv, NULL);
	printf("%lld\t%d\tdied\n", time_ms(&tv) - \
		time_ms(&stuff->tv_start), i);
}

void	wait_child(t_stuff *stuff)
{
	int	child_pid;
	int	exit_child_val;
	int	i;

	i = 1;
	while (true)
	{
		child_pid = waitpid(-1, &exit_child_val, 0);
		if (exit_child_val != 0)
		{
			philo_died(stuff, child_pid);
			clean_up(stuff);
			exit(EXIT_FAILURE);
		}
		if (stuff->must_eat != 0 && i == stuff->number_of_philos)
		{
			kill_philos(stuff, stuff->number_of_philos);
			clean_up(stuff);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
}
