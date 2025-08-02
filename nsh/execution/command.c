# include "execution.h"

static void	sh_check(t_tree *branch, char *path, char **envp)
{
	int		i;
	int		count;
	char	**argv;
	char	**sh_argv;

	if (ENOEXEC != errno)
		return ;
	i = -1;
	count = branch->data.cmd.n_arg;
	argv = branch->data.cmd.args;
	sh_argv = smalloc(count + 2 * sizeof(char *));
	sh_argv[++i] = ft_strndup("sh", 2);
	sh_argv[++i] = path;
	while (++i < count)
		sh_argv[i] = argv[i - 1];
	sh_argv[i] = NULL;
	execve("/bin/sh", sh_argv, envp);
}

static void execute(t_tree *branch, char **envp)
{
    char    *path;
    char   **argv;

    check_redirection(branch);
    argv = branch->data.cmd.args;
    path = resolve_path(argv[0]);
    execve(path, argv, envp);
    sh_check(branch, path, envp);//is it nessec
    exit_failure("execve");
}

static int fork_then_execute(t_tree *branch, char **envp)
{
    int status;
    pid_t pid;

    status = 0;
    pid = fork();
    if (pid < 0)
        exit_failure("fork");
    if (pid == 0)
        execute(branch, envp);
    waitpid(pid, &status, 0);
    return (WEXITSTATUS(status));
}

int execute_command(t_tree *branch, char **envp, t_mode mode)
{
    if (mode == DEFAULT_MODE)
        return (fork_then_execute(branch, envp));
    execute(branch, envp);
    return (1);
}
