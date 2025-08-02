# include "nsh.h"

static int count_args(t_list *tokens)
{
    int count;

    count = 0;
    while (tokens) 
    {
        count++;
        tokens = tokens->next;
    }
    return count;
}

static char **extract_args(t_list *tokens, int n_arg)
{
    t_token *tok;
    t_list *cur;
    char **args;
    int i;

    i = 0;
    cur = tokens;
    args = smalloc((n_arg + 1) * sizeof(char *));
    while (cur)
    {
        tok = (t_token *)cur->content;
        args[i] = tok->value;
        cur = cur->next;
        i++;
    }
    args[i] = NULL;
    return args;
}

t_tree *new_command_branch(t_list *tokens)
{
    t_tree *node;
    int n_redirs;
    int n_arg;

    node = smalloc(sizeof(t_tree));
    node->type = COMMAND;
    n_redirs = count_redirs(tokens);
    node->data.cmd.n_redirs = n_redirs;
    node->data.cmd.redirs = extract_redirs(&tokens, n_redirs);
    n_arg = count_args(tokens);
    node->data.cmd.n_arg = n_arg;
    node->data.cmd.args = extract_args(tokens, n_arg);
    return (node);
}
