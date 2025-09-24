/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_paths_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 11:48:10 by pecastro          #+#    #+#             */
/*   Updated: 2025/09/24 09:03:29 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

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

char	**ft_complete_paths(t_pipex *pipex, int index_cmd)
{
	char	**complete_paths;
	int		i;

	complete_paths = malloc(sizeof(char *) * (pipex->path_count + 1));
	if (!complete_paths)
		return (ft_clean_arr(pipex->path_raw_arr), NULL);
	i = 0;
	while (pipex->path_raw_arr[i])
	{
		if (!pipex->cmd_rel[index_cmd])
			complete_paths[i] = ft_strjoin(pipex->path_raw_arr[i], "/");
		else
			complete_paths[i] = ft_strjoin(pipex->path_raw_arr[i], pipex->cmd_rel[index_cmd]);
		if (!complete_paths[i])
			return (ft_clean_arr_partial(i, complete_paths), NULL);
		i ++;
	}
	complete_paths[i] = NULL;
	return (complete_paths);
}

int	ft_access_paths(t_pipex *pipex)
{
	int	i;
	int	j;
	int	sum_flag;
	int	*flag;

	sum_flag = 0;
	flag = malloc(sizeof(int) * pipex->cmd_count);
	if (!flag)
		return (1);
	i = 0;
	while (i < pipex->cmd_count)
	{
		flag[i] = 1;
		i ++;
	}
	i = 0;
	while (i < pipex->cmd_count)
	{
		j = 0;
		while (pipex->complete_paths[i][j])
		{
			if (access(pipex->complete_paths[i][j], X_OK) == 0)
			{
				pipex->path[i] = pipex->complete_paths[i][j];
				flag[i] = 0;
				break ;
			}
			j ++;
		}
		if (flag[i])
			perror("Error: access()");
		sum_flag += flag[i];
		i ++;
	}
	free (flag);
	return (sum_flag);
}

int	ft_size_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		i ++;
	return (i);
}
