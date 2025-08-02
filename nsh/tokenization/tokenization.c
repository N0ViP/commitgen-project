/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 03:38:34 by yjaafar           #+#    #+#             */
/*   Updated: 2025/08/01 06:12:07 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenization.h"

static int	get_word_len(char *str)
{
	int		j;
	char	quote;

	j = 0;
	while (str[j] && !ft_isspace(str[j]) && !ft_strchr("|><()", str[j]))
	{
		if (str[j] == '&' && str[j + 1] == '&')
		{
			break ;
		}
		if (str[j] == '"' || str[j] == '\'')
		{
			quote = str[j++];
			while (str[j] && str[j] != quote)
			{
				j++;
			}
			if (str[j] != quote)
			{
				return (-1);
			}
		}
		j++;
	}
	return (j);
}

static bool	creat_token(t_list_info *token_info, char *cmd,
						enum e_operator operator, int idx)
{
	t_list	*node;
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		return (false);
	}
	token->type = operator;
	token->value = ft_substr(cmd, 0, idx);
	node = creat_node(token);
	if (!token->value || !node)
	{
		free(token->value);
		free(node);
		return (false);
	}
	node->content = token;
	token_info->size++;
	list_add_back(token_info, node);
	return (true);
}

static int	extract_token_and_type(t_list_info *token_info, char *cmd)
{
	int				j;
	enum e_operator	operator;

	j = 1;
	operator = check_token(cmd);
	if (operator == WORD)
	{
		j = get_word_len(cmd);
		if (j == -1)
		{
			return (free(cmd), free_list(&token_info, free_token),
				write(2, "unclosed quotes\n", 16),-1);
		}
	}
	else if (operator == OP_OR || operator == OP_AND
		|| operator == OP_APPEND || operator == OP_HEREDOC)
		j = 2;
	if (!creat_token(token_info, cmd, operator, j))
	{
		free(cmd);
		free_list(&token_info, free_token);
		exit (EXIT_FAILURE);
	}
	return (j);
}

t_list_info	*tokenize(char *cmd)
{
	t_list_info	*token_info;
	int			i;
	int			j;

	i = 0;
	i += skip_spaces(cmd, i);
	if (!cmd[i])
		return (NULL);
	token_info = init_list_info_struct();
	if (!token_info)
	{
		free(cmd);
		exit(EXIT_FAILURE);
	}
	while (cmd[i])
	{
		j = extract_token_and_type(token_info, cmd + i);
		if (j == -1)
			return (NULL);
		i += j;
		i += skip_spaces(cmd, i);
	}
	return (token_info);
}
