/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:03:21 by pecastro          #+#    #+#             */
/*   Updated: 2025/08/31 10:28:30 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
# include "pipex.h" 

int	main(int argc, char **argv)
{
	t_pipex	pipex;
	int		i;
	int		status_code;

	if (argc != 5)
		return(ft_dprintf(STDERR_FILENO, "./pipex file1 cmd1 cmd2 file2\n"), 1);
	pipex.file[0] = argv[1];
	pipex.file[1] = argv[argc - 1];
	if (!ft_contains_words(argv[2]) || !ft_contains_words(argv[3]))
			return(ft_dprintf(STDERR_FILENO, "Error: empty command\n"), 1);
	i = 0;
	while (i < argc - 3)
	{
		pipex.cmd[i] = ft_split(argv[i + 2], ' ');
		pipex.cmd_rel[i] = ft_strjoin("/", pipex.cmd[i][0]);
		pipex.complete_paths[i] = ft_complete_paths(pipex.cmd_rel[i]);
		pipex.path[i] = ft_access_paths(pipex.complete_paths[i]);
		if (!pipex.path[i])
			return (ft_free(i, &pipex), 1);
		i ++;
	}
	status_code = ft_processes(&pipex);
	return (ft_free(i - 1, &pipex), status_code);
}

char	ft_contains_words(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return (str[i]);
		i ++;
	}
	return ('\0');
}

char	**ft_complete_paths(char *cmd)
{
	char	**path_raw_arr;
	char	**paths;
	int		path_count;
	int		i;

	path_raw_arr = ft_paths_from_environ();
	if (!path_raw_arr)
		return (NULL);
	path_count = ft_size_arr(path_raw_arr);
	paths = malloc(sizeof(char *) * (path_count + 1));
	if (!paths)
		return(ft_clean_arr(path_raw_arr), NULL);
	i = 0;
	while (path_raw_arr[i])
	{
		paths[i] = ft_strjoin(path_raw_arr[i], cmd);
		if (!paths[i])
			return (ft_clean_arr_partial(i, paths), NULL);
		i ++;
	}
	paths[i] = NULL;
	return (ft_clean_arr(path_raw_arr), paths);
}

char	**ft_paths_from_environ(void)
{
	int		i;
	char	*search_word;

	search_word = "PATH=";
	i = 0;
	while (environ[i])
	{
		if (ft_strncmp(environ[i], search_word, ft_strlen(search_word)) == 0)
			return (ft_split(environ[i] + 5, ':'));
		i ++;
	}
	return (NULL);
}

char	*ft_access_paths(char **complete_paths)
{
	int	i;

	i = 0;
	while (complete_paths[i])
	{
		if (access(complete_paths[i], X_OK) == 0)
			return (complete_paths[i]);
		i ++;
	}
	perror("Error: access()");
	return (NULL);
}
