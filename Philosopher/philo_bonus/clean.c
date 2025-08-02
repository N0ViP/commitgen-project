/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:20:22 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 16:22:58 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	clean_up(t_stuff *stuff)
{
	sem_close(stuff->lock);
	sem_close(stuff->forks);
	sem_unlink("/forks");
	sem_unlink("/lock");
	free(stuff->philos);
}

void	clean_sems(t_stuff *stuff)
{
	sem_close(stuff->alive_protection);
	sem_close(stuff->time_protection);
	sem_close(stuff->eat_protection);
	sem_unlink(stuff->alive_protection_name);
	sem_unlink(stuff->time_protection_name);
	sem_unlink(stuff->eat_protection_name);
	free(stuff->alive_protection_name);
	free(stuff->time_protection_name);
	free(stuff->eat_protection_name);
	clean_up(stuff);
}
