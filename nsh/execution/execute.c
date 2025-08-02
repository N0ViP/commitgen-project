# include "execution.h"
//<2

int execution_mode(t_tree *root, char **envp, t_mode mode)
{
    if (!root)
        return (0);
    else if (root->type == COMMAND)
        return (execute_command(root, envp, mode));
    else if (root->type == SUBSHELL)
        return (execute_subshell(root, envp));
    else if (root->type == OP_PIPE)
        return (execute_pipeline(root, envp));
    else if (root->type == OP_OR || root->type == OP_AND)
        return (execute_or_and(root, envp));
    return (1);
}

int execute_tree(t_tree *root, char **envp)
{
    return (execution_mode(root, envp, DEFAULT_MODE));
}
