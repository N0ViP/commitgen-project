/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stdfcts.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:40:51 by yjaafar           #+#    #+#             */
/*   Updated: 2025/08/01 05:22:49 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STDFCTS_H
# define STDFCTS_H

# include <stdbool.h>
# include <stddef.h>
# include <stdlib.h>

bool	ft_isdigit(int a);
bool	ft_isalpha(int a);
bool	ft_isalnum(int a);
bool	ft_isspace(int c);
size_t  ft_strlen(const char *s);
int 	ft_strcmp(char *s1, char *s2);
char	*ft_strcat(char *dest, char *src);
char	*ft_strndup(const char *s, size_t n);
char	*ft_strjoin(char **arr, char *delemiter);
char	**ft_split(const char *s, const char *sep);

#endif