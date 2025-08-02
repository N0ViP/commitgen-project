/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nsh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:40:39 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/31 16:39:12 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NSH_H
# define NSH_H

# include "tokenization.h"
# include "parse_tree.h"
# include "execution.h"

# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <readline/readline.h>
# include <readline/history.h>

#define SHELL "nsh"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_list_info
{
	t_list		*list;
	t_list		*tail;
	int			size;
}	t_list_info;

typedef struct s_token
{
	char			*value;
	enum e_operator	type;
}	t_token;

#endif
