/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:19:37 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 16:20:30 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
