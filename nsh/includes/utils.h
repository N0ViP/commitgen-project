/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:41:01 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/26 08:36:06 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "nsh.h"
# include <stdlib.h>

typedef struct s_list		t_list;
typedef struct s_list_info	t_list_info;

void            _free(char **str);
enum e_operator	check_token(char *token);
t_list			*creat_node(void *content);
int				skip_spaces(char *str, int i);
t_list_info	    *init_list_info_struct();
char			*ft_strchr(const char *str, char c);
char			*ft_substr(char *str, int i, int j);
void			list_add_back(t_list_info *list_info, t_list *node);
void			free_list(t_list_info **list_info, void (*free_content)());

#endif
