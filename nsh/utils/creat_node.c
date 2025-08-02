/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_node.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:38:17 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/01 08:38:19 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

t_list	*creat_node(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	node->content = content;
	node->next = NULL;
	return (node);
}
