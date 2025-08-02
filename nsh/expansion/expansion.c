/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 02:42:03 by yjaafar           #+#    #+#             */
/*   Updated: 2025/08/01 14:18:31 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "expansion.h"

size_t	get_normal_word(char *str, t_list_info *value)
{
	size_t	n;
	char	*word;
	t_list	*node;

	n = 0;
	while (str[n] != '\0' && str[n] != '\''
		&& str[n] != '"' && (str[n] != '$'
			|| !ft_isalpha(str[n + 1]) || str[n + 1] != '_'))
	{
		n++;
	}
	if (n != 0)
	{
		word = ft_substr(str, 0, n);
		node = creat_node(word);
		list_add_back(value, node);
	}
	return (n);
}

size_t	get_single_quote_word(char *str, t_list_info *value)
{
	size_t	n;
	char	*word;
	t_list	*node;

	n = 1;
	if (*str != '\'')
		return (0);
	while (str[n] != '\0' && str[n] != '\'')
	{
		n++;
	}
	word = ft_substr(str, 1, n);
	node = creat_node(word);
	list_add_back(value, node);
	return (n + 1);
}

int	get_double_quote_word(char *str, t_list_info *value)
{
	int	n;
}

void	handle_val(t_list_info *value, char *val)
{
	char	**splited_val;
	t_list	*node;
	
	splited_val = ft_split(val, " \t\n");
	free(val);
	val = ft_strjoin(splited_val, " ");
	node = creat_node(val);
	list_add_back(value, node);
}

void	get_add_dollar_expand(t_list_info *value, char *str, int i, int j)
{
	char	*key;
	char	*val;

	key = ft_substr(str, i, j);
	val = get_var_value(key);
	free(key);
	if (val)
	{
		handle_val(value, val);
	}
}

size_t	get_dollar_word(char *str, t_list_info *value)
{
	size_t		n;

	n = 1;
	if (str[0] != '$' || str[1] == '\0' || str[1] == '$')
	{
		return (0);
	}
	if (ft_isdigit(str[n]))
	{
		while (str[n] != '\0' && (ft_isalnum(str[n]) || str[n] == '_'))
		{
			n++;
		}
	}
	if (n != 1)
	{
		get_add_dollar_expand(value, str, 1, n);
	}
	return (n);
}

t_list_info	*expander(char *str, t_list_info *expand)
{
	t_list_info	*value;
	size_t		n;

	value = init_list_info_struct();
	if (!value)
		return (NULL);
	while (*str)
	{
		n = get_normal_word(str, value);
		str += n;
		n = get_single_quote_word(str, value);
		str += n;
		n = get_double_quote_word(str, value);
		str += n;
		n = get_dollar_word(str, value);
		str += n;
	}
	return (value);
}

char **expansion(t_list *lst)
{
	t_list_info	*expand;
	char		**res;
	
	expand = init_list_info_struct();
	if (!expand)
		return (NULL);
	while (lst)
	{
		if (expander(((t_token *)lst->content)->value, expand))
			return (free(expand), NULL);
		lst = lst->next;
	}
	res = create_array(expand);
	return (res);
}
