/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:21:37 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 17:54:51 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	take_fork(t_stuff *stuff)
{
	sem_wait(stuff->forks);
	if (!is_alive(stuff))
		return ;
	print(stuff, "has taken a fork\n");
}

void	take_forks(t_stuff *stuff)
{
	take_fork(stuff);
	take_fork(stuff);
}

void	put_forks(t_stuff *stuff)
{
	sem_post(stuff->forks);
	sem_post(stuff->forks);
}

void	open_semaphores(t_stuff *stuff)
{
	sem_unlink(stuff->alive_protection_name);
	sem_unlink(stuff->time_protection_name);
	sem_unlink(stuff->eat_protection_name);
	stuff->alive_protection = sem_open(stuff->alive_protection_name, \
		O_CREAT, 0777, 1);
	stuff->time_protection = sem_open(stuff->time_protection_name, \
		O_CREAT, 0777, 1);
	stuff->eat_protection = sem_open(stuff->eat_protection_name, \
		O_CREAT, 0777, 1);
	if (!stuff->alive_protection
		|| !stuff->time_protection
		|| !stuff->eat_protection)
	{
		clean_sems(stuff);
		exit(EXIT_FAILURE);
	}
}

void	init_semaphores(t_stuff *stuff)
{
	stuff->alive_protection_name = ft_strjoin("alive_protection_", \
		ft_itoa(stuff->philo_id));
	stuff->time_protection_name = ft_strjoin("time_protection_", \
		ft_itoa(stuff->philo_id));
	stuff->eat_protection_name = ft_strjoin("eat_protection_", \
		ft_itoa(stuff->philo_id));
	if (!stuff->alive_protection_name
		|| !stuff->time_protection_name
		|| !stuff->eat_protection_name)
	{
		free (stuff->alive_protection_name);
		free (stuff->time_protection_name);
		free (stuff->eat_protection_name);
		clean_up(stuff);
		exit (EXIT_FAILURE);
	}
	open_semaphores(stuff);
}
