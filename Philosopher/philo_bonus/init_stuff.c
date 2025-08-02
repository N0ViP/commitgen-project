/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_stuff.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:03:34 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 16:20:55 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	init_stuff(t_stuff *stuff, int ac, char *av[])
{
	if (ac != 5 && ac != 6)
		return (write(2, "Invalid arguments!\n", 19), false);
	stuff->alive = true;
	stuff->must_eat = 0;
	stuff->number_of_philos = ft_atoi(av[1]);
	stuff->t_to_die = ft_atoi(av[2]);
	stuff->t_to_eat = ft_atoi(av[3]);
	stuff->t_to_sleep = ft_atoi(av[4]);
	if (stuff->number_of_philos <= 0
		|| stuff->t_to_die <= 0
		|| stuff->t_to_eat <= 0
		|| stuff->t_to_sleep <= 0)
		return (write(2, "Invalid arguments!\n", 19), false);
	if (ac == 6)
	{
		stuff->must_eat = ft_atoi(av[5]);
		if (stuff->must_eat <= 0)
			return (false);
	}
	return (true);
}
