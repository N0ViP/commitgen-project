/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:21:26 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 18:04:01 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	print(t_stuff *stuff, char *str)
{
	struct timeval	tv;

	sem_wait(stuff->lock);
	gettimeofday(&tv, NULL);
	printf("%lld\t%d\t%s", time_ms(&tv) - \
		time_ms(&stuff->tv_start), stuff->philo_id, str);
	sem_post(stuff->lock);
}

long long	time_ms(struct timeval *tv)
{
	return ((long long)(tv->tv_sec * 1000) + (tv->tv_usec / 1000));
}

int	ft_abs(int x)
{
	if (x < 0)
		return (-x);
	return (x);
}

bool	is_alive(t_stuff *stuff)
{
	bool			alive;

	sem_wait(stuff->alive_protection);
	alive = stuff->alive;
	sem_post(stuff->alive_protection);
	return (alive);
}
