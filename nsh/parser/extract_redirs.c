# include "nsh.h"

int count_redirs(t_list *tokens)
{
    t_token *tok;
    t_list *cur;
    int count;

    cur = tokens;
    count = 0;
    while (cur)
    {
        tok = (t_token *)cur->content;
        if (tok->type >= OP_APPEND && tok->type <= OP_REDIR_IN)
            count++;
        cur = cur->next;
    }
    return count;
}

static void recurse_redirs(t_list **cur, t_redir *redirs, int i)
{
    t_token *tok;
    t_token *file_tok;
    t_list  *file_node;

    if (!*cur)
        return;
    tok = (t_token *)(*cur)->content;
    if (tok->type >= OP_APPEND && tok->type <= OP_REDIR_IN)
    {
        file_node = (*cur)->next;
        if (!file_node)
            return ;
        file_tok = (t_token *)file_node->content;
        redirs[i].type = tok->type;
        redirs[i++].file = file_tok->value;
        *cur = file_node->next;
    }
    else
        cur = &((*cur)->next);
    return (recurse_redirs(cur, redirs, i));
}

t_redir *extract_redirs(t_list **tokens, int n_redirs)
{
    t_redir *redirs;

    if (!n_redirs)
        return (NULL);
    redirs = smalloc(sizeof(t_redir) * n_redirs);
    recurse_redirs(tokens, redirs, 0);
    return (redirs);
}