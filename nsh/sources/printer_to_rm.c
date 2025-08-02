# include "nsh.h"
# include "string.h"

const char *operator_type_name(enum e_operator type) {
    switch (type) {
        case WORD:              return "WORD";
        case OP_OR:             return "OP_OR";
        case OP_AND:            return "OP_AND";
        case OP_APPEND:         return "OP_APPEND";
        case OP_HEREDOC:        return "OP_HEREDOC";
        case OP_REDIR_OUT:      return "OP_REDIR_OUT";
        case OP_REDIR_IN:       return "OP_REDIR_IN";
        case OP_OPEN_PARENTHESE:return "OP_OPEN_PARENTHESE";
        case OP_CLOSED_PARENTHESE:return "OP_CLOSED_PARENTHESE";
        case OP_PIPE:           return "OP_PIPE";
        case SUBSHELL:          return "SUBSHELL";
        case COMMAND:           return "COMMAND";
        default:                return "UNKNOWN";
    }
}

void print_tokens(t_list *list)
{
	if (!list) return;
    while (list)
    {
        t_token *token = (t_token *)list->content;
        printf("TOKEN___%s_______%s\n", token->value, operator_type_name(token->type));
        list = list->next;
    }
}

void tree_printer_00(t_tree *root, int indent)
{
    if (!root) return;
    for (int i = 0; i < indent; i++) putchar(' ');

    switch (root->type) {
        case COMMAND:
            printf("CMD: ");
            for (int i = 0; i < root->data.cmd.n_arg; i++)
                printf(" %s", root->data.cmd.args[i]);
            printf("\n");
            for (int i = 0; i < root->data.cmd.n_redirs; i++)
            {
                if(i == 0)
                {
                    for (int j = 0; j < indent; j++) putchar(' ');
                    printf("REDIRS:");
                }
                t_redir *r = &root->data.cmd.redirs[i];
                if(!r)
                    return;
                switch (r->type) {
                    case OP_REDIR_IN:     printf(" <%s", r->file); break;
                    case OP_REDIR_OUT:    printf(" >%s", r->file); break;
                    case OP_APPEND:       printf(" >>%s", r->file); break;
                    case OP_HEREDOC:      printf(" >>%s", r->file); break;
                    default: printf("Unknown redir type"); break;
                }
            }
            if(root->data.cmd.n_redirs)
                printf("\n");
            break;

        case OP_PIPE:
            printf("PIPE\n");
            tree_printer_00(root->data.branch.left, indent + 3);
            tree_printer_00(root->data.branch.right, indent + 3);
            break;

        case OP_OR:
            printf("OR (||)\n");
            tree_printer_00(root->data.branch.left, indent + 3);
            tree_printer_00(root->data.branch.right, indent + 3);
            break;

        case OP_AND:
            printf("AND (&&)\n");
            tree_printer_00(root->data.branch.left, indent + 3);
            tree_printer_00(root->data.branch.right, indent + 3);
            break;

        case SUBSHELL:
            printf("SUBSHELL\n");
            tree_printer_00(root->data.subshell.child, indent + 3);
            for (int i = 0; i < root->data.subshell.n_redirs; i++)
            {
                if(i == 0)
                {
                    for (int j = 0; j < indent + 3; j++) putchar(' ');
                    printf("REDIRS:");
                }
                t_redir *r = &root->data.subshell.redirs[i];
                if(!r)
                    return;
                switch (r->type) {
                    case OP_REDIR_IN:     printf(" <%s", r->file); break;
                    case OP_REDIR_OUT:    printf(" >%s", r->file); break;
                    case OP_APPEND:       printf(" >>%s", r->file); break;
                    case OP_HEREDOC:      printf(" >>%s", r->file); break;
                    default: printf("Unknown redir type"); break;
                }
            }
            if(root->data.subshell.n_redirs)
                printf("\n");
            break;

        default:
            printf("Unknown node type\n");
            break;
    }
}


void tree_printer_01(t_tree *node, const char *prefix, bool is_left, bool is_first)
{
    if (!node) 
        return;

    char label[512] = {0};
    int n = 0;
    t_redir *rd = NULL;

    switch (node->type)
    {
        case COMMAND:
            strcpy(label, "COMMAND:");
            for (int i = 0; i < node->data.cmd.n_arg; i++)
            {
                strcat(label, " ");
                strcat(label, node->data.cmd.args[i]);
            }
            n  = node->data.cmd.n_redirs;
            rd = node->data.cmd.redirs;
            break;

        case SUBSHELL:
            strcpy(label, "SUBSHELL");
            n  = node->data.subshell.n_redirs;
            rd = node->data.subshell.redirs;
            break;

        case OP_PIPE:  strcpy(label, "PIPE");        break;
        case OP_AND:   strcpy(label, "AND (&&)");    break;
        case OP_OR:    strcpy(label, "OR (||)");     break;
        default:       strcpy(label, "UNKNOWN");     break;
    }

    char new_pref[512];
    strcpy(new_pref, prefix);
    if(!is_first)
        strcat(new_pref, " ");
    printf("%s%s%s\n", new_pref, is_first ? "" : (is_left ? "├── " : "└── "), label);

    if(!is_first)
        strcat(new_pref, is_left ? "│   " : "    ");
    
    is_first = false;

    if (n > 0 && rd)
    {
        char redirs_str[512] = "REDIRS:";
        for (int i = 0; i < n; i++)
        {
            char redir[64];
            switch (rd[i].type)
            {
                case OP_REDIR_IN:  sprintf(redir, " <%s", rd[i].file);  break;
                case OP_REDIR_OUT: sprintf(redir, " >%s", rd[i].file);  break;
                case OP_APPEND:    sprintf(redir, " >>%s", rd[i].file); break;
                case OP_HEREDOC:   sprintf(redir, " <<%s", rd[i].file); break;
                default:           sprintf(redir, " ?%s", rd[i].file);  break;
            }
            strcat(redirs_str, redir);
        }
        if (node->type == COMMAND)
            printf("%s    %s\n", new_pref, redirs_str);
        if (node->type != COMMAND)
            printf("%s │   %s\n", new_pref, redirs_str);
    }

    if (node->type == SUBSHELL)
    {
        tree_printer_01(node->data.subshell.child, new_pref, false, is_first);
    }
    else if (node->type == OP_AND || node->type == OP_OR || node->type == OP_PIPE)
    {
        tree_printer_01(node->data.branch.left,  new_pref, true, is_first);
        tree_printer_01(node->data.branch.right, new_pref, false, is_first);
    }
}