# include "nsh.h"

static int	op_priority(t_type type)
{
	if (type == OP_PIPE)
		return (1);
	return (0);
}

static bool	is_split_point(t_list *split_point, t_type curr_type)
{
	t_type	last_type;

	if (!split_point)
		return (true);
	last_type = ((t_token *)split_point->next->content)->type;
	return (op_priority(curr_type) <= op_priority(last_type));
}

static t_list	*find_split_point(t_list *tokens)
{
	t_list	*split_point;
	t_token	*token;
	t_list	*prev;
	int		depth;

    depth = 0;
    prev = NULL;
    token = NULL;
    split_point = NULL;
	while (tokens)
	{
		token = (t_token *)tokens->content;
		if (token->type == OP_OPEN_PARENTHESE)
			depth++;
		else if (token->type == OP_CLOSED_PARENTHESE)
			depth--;
		else if (!depth && token->type >= OP_OR && token->type <= OP_PIPE)
			if (is_split_point(split_point, token->type))
				split_point = prev;
		prev = tokens;
		tokens = tokens->next;
	}
	return (split_point);
}

t_tree	*parse_tokens(t_list *tokens)
{
	t_list	*split_point;

	split_point = find_split_point(tokens);
	if (split_point)
		return (new_operator_branch(tokens, split_point));
	else if (((t_token *)tokens->content)->type == OP_OPEN_PARENTHESE)
		return (new_subshell_branch(tokens));
	return (new_command_branch(tokens));
}
