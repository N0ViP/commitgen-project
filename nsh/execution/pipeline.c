# include "execution.h"

static pid_t left_pipe(int pipefd[2], t_tree *branch, char **envp)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		exit_failure("fork");
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		exit(execution_mode(branch->data.branch.left, envp, NO_FORK_MODE));
	}
	return (pid);
}

static pid_t right_pipe(int pipefd[2], t_tree *branch, char **envp)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
		exit_failure("fork");
	if (pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		exit(execution_mode(branch->data.branch.right, envp, NO_FORK_MODE));
	}
	return (pid);
}

int execute_pipeline(t_tree *branch, char **envp)
{
	int   pipefd[2];
	pid_t left, right;
	int   status;

	if (pipe(pipefd) < 0)
		exit_failure("pipe");

	left  = left_pipe(pipefd, branch, envp);
	right = right_pipe(pipefd, branch, envp);

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(left,  NULL, 0);
	waitpid(right, &status, 0);
	if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}
