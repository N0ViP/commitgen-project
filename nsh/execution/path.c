# include "execution.h"

static t_state path_validity(const char *cmd)
{
	struct stat	st;

	if (stat(cmd, &st) < 0)
		return (stat_error());
	if (S_ISDIR(st.st_mode))
		return (IS_DIR_ERROR);
	if (access(cmd, X_OK) < 0)
		return (PERMISSION_ERROR);
	return (VALID_PATH);
}

static t_state update_error(t_state state, char **err_path, char *full_path)
{
    if (state == PERMISSION_ERROR)
    {
        free(*err_path);
        *err_path = full_path;
    }
    return (state);
}

static char	*get_path(const char *cmd, char *path)
{
	char	*full_path;
	char	*arr[3];

	arr[0] = path;
	arr[1] = cmd;
	arr[2] = NULL;
	full_path = ft_strjoin(arr, "/");
	return (full_path);
}

static char	*find_in_path(const char *cmd, char **paths, t_state *state)
{
	char		*full_path;
	char		*err_path;
	t_state		error;
	int			i;

	i = -1;
    err_path = NULL;
	error = NOT_FOUND_ERROR;
	while (paths[++i])
	{
        full_path = get_path(cmd ,paths[i]);
		*state = path_validity(full_path);
		if (*state == VALID_PATH)//first that works 
		    return (full_path);
		if (*state != NO_FILE_ERROR)//last err
			error = update_error(*state, &err_path, full_path);
        if(full_path == err_path)
            continue;
		free(full_path);
	}
	*state = error;
    if(*state == PERMISSION_ERROR)
        return (err_path);
    free(err_path);
	return ((char *)cmd);
}

char	*resolve_path(const char *cmd)
{
	t_state		state;
	char		**paths;
	char		*path_env;
	char		*full_path;

	if (ft_strchr(cmd, '/'))
	{
		state = path_validity(cmd);
		if (state == VALID_PATH)
			return (ft_strndup(cmd, ft_strlen(cmd)));
		report_error(cmd, state);
	}
    path_env = getenv("PATH");
	paths = ft_split(path_env, ":");
    if(!paths)
	{
        report_error(cmd, NO_FILE_ERROR);
	}
	full_path = find_in_path(cmd, paths, &state);
    _free(paths);
	if (state != VALID_PATH)
		report_error(full_path, state);
	return (full_path);
}
