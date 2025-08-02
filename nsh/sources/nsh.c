/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nsh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yjaafar <yjaafar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 08:37:28 by yjaafar           #+#    #+#             */
/*   Updated: 2025/07/20 03:31:47 by yjaafar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "nsh.h"

int	main(int argc, char **argv, char**envp)
{
	t_list_info	*tokens;
	char		*cmd;

	(void)argc, (void)argv;
	while (true)
	{
		cmd = readline("\e[32mnsh$\e[0m ");
		if(!cmd)
			exit(0);
		if(!(*cmd))
		{
			free(cmd);
			continue;
		}
		add_history(cmd);
		
		tokens = tokenize(cmd);
		if (!tokens) continue;
		printf("\n-------tokenize-------\n\n");
		print_tokens(tokens->list);

		if (!parse_check(tokens->list))
			continue;
        t_tree *root = parse_tokens(tokens->list);
		printf("\n---------TREE---------\n\n");
        tree_printer_01(root, "", false, true);

		printf("\n-------EXECUTION-------\n\n");
		execute_tree(root, envp);

		free(cmd);
	}
}
