/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 03:38:51 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/02 03:39:20 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

void	free_token(t_list *node)
{
	t_token	*token;

	token = node->content;
	node->content = NULL;
	free(token->value);
}
