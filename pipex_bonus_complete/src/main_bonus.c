/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 14:03:21 by pecastro          #+#    #+#             */
/*   Updated: 2025/09/17 22:26:36 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

int	main(int argc, char **argv)
{
	t_pipex	pipex;
	int		i;
	int		status_code;

	if (argc < 5)
		return (ft_dprintf(STDERR_FILENO, "./pipex file1 cmd1 ... cmdN file2\n"), EXIT_FAILURE);
	pipex.hd = 0;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		pipex.hd = 1;
		pipex.limiter = argv[2];
	}
	if (!pipex.hd)
		pipex.file[0] = argv[1];
	else
		pipex.file[0] = NULL;
	pipex.file[1] = argv[argc - 1];
	pipex.cmd_count = argc - 3 - pipex.hd;
	i = 0;
	while (i < pipex.cmd_count)
	{
		if (!ft_contains_words(argv[i + 2 + pipex.hd]))
		return (ft_dprintf(STDERR_FILENO, "Error: empty command\n"), EXIT_FAILURE);
		i ++;
	}
	pipex.path_raw_arr = ft_paths_from_environ();
	if (!pipex.path_raw_arr)
		return (1);
	pipex.path_count = ft_size_arr(pipex.path_raw_arr);	
	if (ft_pipex_init(&pipex) == 1)
		return (ft_clean_arr(pipex.path_raw_arr), EXIT_FAILURE);
	i = 0;
	while (i < pipex.cmd_count)
	{
		pipex.cmd[i] = ft_split(argv[i + 2 + pipex.hd], ' ');
		pipex.cmd_rel[i] = ft_strjoin("/", pipex.cmd[i][0]);
		pipex.complete_paths[i] = ft_complete_paths(&pipex, i);
		if (!pipex.complete_paths[i])
			return (ft_free(i, &pipex), EXIT_FAILURE);
		i ++;
	}
	status_code = ft_processes(&pipex);
	return (ft_free(i - 1, &pipex), status_code);
}

int	ft_pipex_init(t_pipex *pipex)//2 functions: 1.alloc, 2.free (if exists, free).
{
	pipex->cmd = malloc(sizeof(char **) * pipex->cmd_count);
	if (!pipex->cmd)
		return (1);
	pipex->cmd_rel = malloc(sizeof(char *) * pipex->cmd_count);
	if (!pipex->cmd_rel)
		return (free (pipex->cmd), 1);
	pipex->complete_paths = malloc(sizeof(char **) * pipex->cmd_count);
	if (!pipex->complete_paths)
	{
		free (pipex->cmd);
		free (pipex->cmd_rel);
		return (1);
	}
	pipex->path = malloc(sizeof(char *) * pipex->cmd_count); 
	if (!pipex->path)
	{
		free (pipex->cmd);
		free (pipex->cmd_rel);
		free (pipex->complete_paths);
		return (1);
	}
	pipex->pid = malloc(sizeof(int) * pipex->cmd_count);
	if (!pipex->pid)
	{
		free (pipex->cmd);
		free (pipex->cmd_rel);
		free (pipex->complete_paths);
		free (pipex->path);
		return (1);
	}
	pipex->fd = malloc(sizeof(int[2]) * (pipex->cmd_count - 1));
	if (!pipex->fd)
	{
		free (pipex->cmd);
		free (pipex->cmd_rel);
		free (pipex->complete_paths);
		free (pipex->path);
		free (pipex->pid);
		return (1);
	}
	return (0);
}
