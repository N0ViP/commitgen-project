/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 16:03:08 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/11 16:20:37 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_numlen(int n)
{
	int	i;

	i = 0;
	while (n)
	{
		i++;
		n /= 10;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*res;
	int		len;

	len = ft_numlen(n);
	res = malloc(len + 1);
	res[len--] = 0;
	while (n)
	{
		res[len--] = (n % 10) + 48;
		n /= 10;
	}
	return (res);
}
