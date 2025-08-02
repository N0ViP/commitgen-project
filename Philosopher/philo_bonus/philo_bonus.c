/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:21:05 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 16:21:06 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	main(int ac, char **av)
{
	t_stuff	stuff;

	stuff = (t_stuff){0};
	if (!init_stuff(&stuff, ac, av))
		return (1);
	if (stuff.number_of_philos == 1)
		one_philo(stuff.t_to_die);
	init_philos(&stuff);
	return (0);
}
