# include "execution.h"

static void heredoc(const char *delimiter)
{
    (void)delimiter;
}

static void redirect(const char *file, int flags, int target_fd)
{
    int fd;
    
    fd = open(file, flags, 0644);
    if (fd < 0)
        exit_failure((char *)file);
    if (dup2(fd, target_fd) < 0)
        exit_failure("dup2");
    close(fd);
}

static void setup_redirection(t_redir *redir, int n_redirs)
{
    int i;

    i = 0;
    while(i < n_redirs)
    {
        if (redir[i].type == OP_REDIR_IN)
            redirect(redir[i].file, O_RDONLY, STDIN_FILENO);
        else if (redir[i].type == OP_REDIR_OUT)
            redirect(redir[i].file,
                        O_WRONLY | O_CREAT | O_TRUNC, 
                        STDOUT_FILENO);
        else if (redir[i].type == OP_APPEND)
            redirect(redir[i].file,
                        O_WRONLY | O_CREAT | O_APPEND,
                        STDOUT_FILENO);
        else if (redir[i].type == OP_HEREDOC)
            heredoc(redir[i].file);
        i++;
    }
}

void check_redirection(t_tree *branch)
{
    t_redir *redir;
    int     n_redirs;

    redir = NULL;
    n_redirs = 0;
    if(branch->type == COMMAND)
    {
        redir = branch->data.cmd.redirs;
        n_redirs = branch->data.cmd.n_redirs;
    }
    else if(branch->type == SUBSHELL)
    {
        redir = branch->data.subshell.redirs;
        n_redirs = branch->data.subshell.n_redirs;
    }
    if(!n_redirs)
        return ;
    setup_redirection(redir, n_redirs);
}
