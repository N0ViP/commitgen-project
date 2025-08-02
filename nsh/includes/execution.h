#ifndef EXECUTION_H
# define EXECUTION_H

# include "nsh.h"
# include "utils.h"
# include "stdfcts.h"
# include "errno.h"
# include <sys/stat.h>
# include <sys/wait.h>

typedef enum
{
    VALID_PATH,
    NOT_FOUND_ERROR,
    NO_FILE_ERROR,
    IS_DIR_ERROR,
    PERMISSION_ERROR,
    STAT_ERROR
}   t_state;

typedef enum
{
    DEFAULT_MODE,
    NO_FORK_MODE  
}   t_mode;

int		    execute_command(t_tree *branch, char **envp, t_mode mode);
int		    execution_mode(t_tree *root, char **envp, t_mode mode);
int		    execute_pipeline(t_tree *branch, char **envp);
int		    execute_subshell(t_tree *branch, char **envp);
void        report_error(const char *cmd, t_state error);
int		    execute_or_and(t_tree *branch, char **envp);
int		    execute_tree(t_tree *root, char **envp);
void	    check_redirection(t_tree *branch);
char	    *resolve_path(const char *cmd);
void        exit_failure(char *msg);
t_state     stat_error(void);

#endif