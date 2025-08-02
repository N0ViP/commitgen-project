# include "nsh.h"

static bool	check_open_paren(t_list *tokens, t_token *prev)
{
	t_token *next;

	if (!tokens->next)
		return (parse_error("("), false);
	next = (t_token *)tokens->next->content;
	if (prev
	 && prev->type != OP_OPEN_PARENTHESE
	 && !(prev->type >= OP_OR && prev->type <= OP_PIPE))
		return (parse_error("("), false);
	if ((next->type >= OP_OR && next->type <= OP_REDIR_IN)
	 || next->type == OP_CLOSED_PARENTHESE)
		return (parse_error(next->value), false);
	return (true);
}

static bool	check_close_paren(t_list *tokens, t_token *prev, int *depth)
{
	t_token	*curr;
	t_token	*next;

	curr = (t_token *)tokens->content;
	if (*depth == 0)
		return (parse_error(curr->value), false);
	if (tokens->next)
	{
		next = (t_token *)tokens->next->content;
		if (!((next->type >= OP_OR && next->type <= OP_REDIR_IN)
		 || next->type == OP_CLOSED_PARENTHESE))
			return (parse_error(next->value), false);
	}
	if (prev->type >= OP_OR && prev->type <= OP_REDIR_IN)
		return (parse_error(curr->value), false);
	(*depth)--;
	return (true);
}

static bool	check_ops_and_redirs(t_list *tokens, t_token *prev)
{
	t_token *curr;

	curr = (t_token *)tokens->content;
	if (curr->type >= OP_OR && curr->type <= OP_PIPE)
	{
		if (!prev
		 || !tokens->next
		 || (prev->type >= OP_OR && prev->type <= OP_REDIR_IN))
			return (parse_error(curr->value), false);
	}
	else if (curr->type >= OP_APPEND && curr->type <= OP_REDIR_IN)
	{
		if (!tokens->next)
			return (parse_error("newline"), false);
	}
	return (true);
}

static bool	process_token(t_list *tokens, t_token *prev, int *depth)
{
	t_type curr_type;

	curr_type = ((t_token *)tokens->content)->type;
	if (curr_type == OP_OPEN_PARENTHESE)
	{
		(*depth)++;
		if (!check_open_paren(tokens, prev))
			return (false);
	}
	else if (curr_type == OP_CLOSED_PARENTHESE)
	{
		if (!check_close_paren(tokens, prev, depth))
			return (false);
	}
	else
	{
		if (!check_ops_and_redirs(tokens, prev))
			return (false);
	}
	return (true);
}

bool	parse_check(t_list *tokens)
{
	t_token	*prev;
	int		depth;

	depth = 0;
	prev = NULL;
	while (tokens)
	{
		if (!process_token(tokens, prev, &depth))
			return (false);
		prev = (t_token *)tokens->content;
		tokens = tokens->next;
	}
	if (depth > 0)
		return (parse_error("("), false);
	return (true);
}
