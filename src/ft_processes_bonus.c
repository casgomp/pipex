/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_processes_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pecastro <pecastro@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 14:05:11 by pecastro          #+#    #+#             */
/*   Updated: 2025/09/24 15:36:00 by pecastro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pipex_bonus.h"

int	ft_processes(t_pipex *pipex)
{
	int		*wstatus;
	int		status_code;
	int		i;

	if (ft_children(pipex) > 0)
		return (EXIT_FAILURE);
	if (!pipex->hd)
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
		return (EXIT_FAILURE);
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

	if (pipex->hd)
	{
		if (pipe(pipex->fd_hd) == -1)
			return (perror("Error: open()"), EXIT_FAILURE);
		if (ft_heredoc(pipex->fd_hd[1], pipex->limiter) > 0)
			return (perror("Error: heredoc"), EXIT_FAILURE); //check return statement from shell.
		close (pipex->fd_hd[1]);
		pipex->fd_f[0] = pipex->fd_hd[0];
	}
	else
		pipex->fd_f[0] = open(pipex->file[0], O_RDONLY);
	if (pipex->fd_f[0] == -1)
		return (perror("Error: open()"), EXIT_FAILURE);
	if (pipex->hd)
		pipex->fd_f[1] = open(pipex->file[1], O_CREAT | O_APPEND | O_WRONLY, 0644);
	else	
		pipex->fd_f[1] = open(pipex->file[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (pipex->fd_f[1] == -1)
		return (perror("Error: open()"), EXIT_FAILURE);
	ft_access_paths(pipex);
	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		if (pipe(pipex->fd[i]) == -1)
	  		return (perror("Error: pipe()"), EXIT_FAILURE);
		i ++;
	}
	i = 0;
	while (i < pipex->cmd_count)
	{
		pipex->pid[i] = fork();
		if (pipex->pid[i] == -1)
			return (perror("Error: fork()"), EXIT_FAILURE);
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
	if (pipex->path[index])
	{
		execve(pipex->path[index], pipex->cmd[index], environ);
		perror("Error: execve()");
	}
	else
	{
		close(STDOUT_FILENO);
		ft_dprintf(2, "Command not found: %s\n", pipex->cmd[index][0]);
	}
	ft_free(pipex->cmd_count - 1, pipex);
	exit (1);
}

int	ft_heredoc(int fd, char *limiter)
{
	char	*ret;
	char	*prompt;
	int		len;

	len = ft_strlen(limiter);
	prompt = "pipe heredoc > ";
	while (1)
	{
		if (write(1, prompt, ft_strlen(prompt)) == -1) //use ft_putstr
			return (1);
		ret = get_next_line(0);
		if (!ret)
			return (1);
		if (ft_strncmp(ret, limiter, len) == 0 && ret[len] == '\n')
			return (free(ret), 0);
		if (write(fd, ret, ft_strlen(ret)) == -1) //use ft_putstr
			return (free(ret), 1);
		free (ret);
	}
}
