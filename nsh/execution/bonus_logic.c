# include "execution.h"

int execute_subshell(t_tree *branch, char **envp)
{
    pid_t pid;
    int status;
    t_tree *child;

    pid = fork();
    if (pid < 0)
        exit_failure("fork");
    child = branch->data.subshell.child;
    if (pid == 0)
    {
        check_redirection(branch);
        status = execution_mode(child, envp, NO_FORK_MODE);
        exit(status);
    }
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}

int execute_or_and(t_tree *branch, char **envp)
{
    t_tree *sub_branch[2];
    int left_status;

    sub_branch[0] = branch->data.branch.left;
    sub_branch[1] = branch->data.branch.right;
    left_status = execution_mode(sub_branch[0], envp, DEFAULT_MODE);
    if ((branch->type == OP_AND && left_status == 0) ||
        (branch->type == OP_OR  && left_status != 0))
        return execution_mode(sub_branch[1], envp, DEFAULT_MODE);
    return left_status;
}
