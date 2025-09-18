/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 16:29:07 by pecastro          #+#    #+#             */
/*   Updated: 2025/09/18 17:40:34 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <unistd.h>
# include "../lib/libft/include/libft.h"

typedef struct s_pipex
{
	int		hd;
	char	*limiter;
	char	*file[2];
	int		cmd_count;
	char	**path_raw_arr;
	int		path_count;
	char	***cmd;
	char	**cmd_rel;
	char	***complete_paths;
	char	**path;
	int		fd_hd[2];
	int		(*fd)[2];
	int		fd_f[2];
	int		*pid;
}t_pipex;

extern char	**environ;

//main
int		main(int argc, char **argv);
int		ft_pipex_init(t_pipex *pipex);
void	ft_pipex_init_free(t_pipex *pipex, int index);
//ft_paths
char	**ft_complete_paths(t_pipex *pipex, int index_cmd);
char	**ft_paths_from_environ(void);
int		ft_access_paths(t_pipex *pipex);
char	ft_contains_words(char *str);
int		ft_size_arr(char **arr);
//ft_processes
int		ft_processes(t_pipex *pipex);
int		ft_children(t_pipex *pipex);
void	ft_process(t_pipex *pipex, int index);
int		ft_heredoc(int fd, char *limiter);
//ft_clean
void	ft_clean_arr_partial(int index, char **arr);
void	ft_clean_arr(char **arr);
void	ft_free(int index, t_pipex *pipex);

#endif
