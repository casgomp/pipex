/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_processes_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:05:11 by pecastro          #+#    #+#             */
/*   Updated: 2025/09/16 19:49:54 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

int	ft_processes(t_pipex *pipex)
{
	int		*wstatus;
	int		status_code;
	int		i;

	if (ft_children(pipex) == 1)
		return (1);
	close (pipex->fd_f[0]);
	close (pipex->fd_f[1]);
	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		close (pipex->fd[i][0]);
		close (pipex->fd[i][1]);
		i ++;
	}
	status_code = 0;
	wstatus = malloc(sizeof(int) * pipex->cmd_count);
	if (!wstatus)
		return (1);
	i = 0;
	while (i < pipex->cmd_count)
	{
		waitpid (pipex->pid[i], &wstatus[i], 0);
		if (WIFEXITED(wstatus[i]))
			status_code = WEXITSTATUS(wstatus[i]);
		i ++;
	}
	free (wstatus);
	return (status_code);
}

int	ft_children(t_pipex *pipex)
{
	int	i;

	pipex->fd_f[0] = open(pipex->file[0], O_RDONLY);
	if (pipex->fd_f[0] == -1)
		return (perror("Error: open()"), 1);
	pipex->fd_f[1] = open(pipex->file[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (pipex->fd_f[1] == -1)
		return (perror("Error: open()"), 1);
	if (ft_access_paths(pipex) > 0)
		return (1);
	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		if (pipe(pipex->fd[i]) == -1)
	  		return (perror("Error: pipe()"), 1);
		i ++;
	}
	i = 0;
	while (i < pipex->cmd_count)
	{
		pipex->pid[i] = fork();
		if (pipex->pid[i] == -1)
			return (perror("Error: fork()"), 1);
		if (pipex->pid[i] == 0)
			ft_process(pipex, i);
		i ++;
	}
	return (0);
}

void	ft_process(t_pipex *pipex, int index)
{
	int	i;

	if (index == 0)
	{
		dup2(pipex->fd_f[0], STDIN_FILENO);
		dup2(pipex->fd[0][1], STDOUT_FILENO);
	}
	else if (index == pipex->cmd_count - 1)
	{
		dup2(pipex->fd[index - 1][0], STDIN_FILENO);
		dup2(pipex->fd_f[1], STDOUT_FILENO);
	}
	else
	{
		dup2(pipex->fd[index - 1][0], STDIN_FILENO);
		dup2(pipex->fd[index][1], STDOUT_FILENO);
	}
	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		close(pipex->fd[i][0]);
		close(pipex->fd[i][1]);
		i ++;
	}
	close(pipex->fd_f[0]);
	close(pipex->fd_f[1]);
	execve(pipex->path[index], pipex->cmd[index], environ);
	perror("Error: execve()");
	exit (1);
}
