#include "../../inc/minishell.h"

static void	one_command(t_msh *msh)
{
	if (is_builtin(msh->cmds[0].main))
	{
		/* Ejecutar el built-in */
		exe_built_ins(msh);
	}
    else
    {
		/* Ejecutar un comando alone */
		exe_one_cmd(msh);
    }
}
void	wait_childs(t_msh *msh)
{
	pid_t	current_pid;
	int		status;

	while (1)
	{
		current_pid = waitpid(-1, &status, 0);
		if (current_pid <= 0)
			break ;
		if (current_pid == msh->final_pid)
		{
			if (WEXITSTATUS(status))
				msh->last_status = WEXITSTATUS(status);
		}
	}
}

static void	two_or_more_cmds(t_msh *msh)
{
	int		new[2];
	int		fd[2];
	int		counter;

	if (pipe(fd) < 0)
		exit_fork_pipe(PIPE);
	first_child(msh, fd, &msh->cmds[0]);
	close(fd[1]);
	counter = 1;
	while (counter < (msh->cmds_count - 2))
	{
		if (pipe(new) < 0)
			exit_fork_pipe(PIPE);
		mid_child(msh, fd, new, &msh->cmds[counter]);
		close(fd[0]);
		close(new[1]);
		fd[0] = new[0];
		counter++;
	}
	last_child(msh, fd, &msh->cmds[msh->cmds_count - 1]);
	close(new[0]);
	close(new[1]);
	close(fd[0]);
	close(fd[1]);
	wait_childs(msh);
}

int	executor(t_msh *msh)
{
	/* Miramos el numero de comandos que hay que ejecutar */
	if (msh->cmds_count == 1)
	{
		/* Solo ejecutamos un comando */
        one_command(msh);
        return (0);
	}
	else
	{
		/* Ejecutamos varios comandos */
		two_or_more_cmds(msh);
		return (0);
	}
}