/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 06:38:22 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/03 06:38:39 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

enum e_operator	check_token(char *token)
{
	enum e_operator	type;

	if (token[0] == '|' && token[1] == '|')
		type = OP_OR;
	else if (token[0] == '&' && token[1] == '&')
		type = OP_AND;
	else if (token[0] == '<' && token[1] == '<')
		type = OP_HEREDOC;
	else if (token[0] == '>' && token[1] == '>')
		type = OP_APPEND;
	else if (token[0] == '|')
		type = OP_PIPE;
	else if (token[0] == '>')
		type = OP_REDIR_OUT;
	else if (token[0] == '<')
		type = OP_REDIR_IN;
	else if (token[0] == '(')
		type = OP_OPEN_PARENTHESE;
	else if (token[0] == ')')
		type = OP_CLOSED_PARENTHESE;
	else
		type = WORD;
	return (type);
}
