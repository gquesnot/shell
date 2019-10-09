#include "minishell.h"

void		redirect(t_chev *chev)
{
	int		out;

	if (chev->set.out != 0)
	{
		chev->save_out = dup(STDOUT_FILENO);
		chev->save_in = dup(STDIN_FILENO);
		if (chev->set.out == 1)
			out = open(chev->str_chev_out, O_CREAT | O_TRUNC | O_WRONLY);
		if (chev->set.out == 2)
			out = open(chev->str_chev_out, O_CREAT | O_APPEND | O_WRONLY);
	}
	else
		return;
	dup2(out, STDOUT_FILENO);
	close(out);
}

void		redirect_in(t_chev *chev)
{
	int		in;

	chev->save_in = dup(STDIN_FILENO);
	if (chev->set.in == 1)
		in = open(chev->str_chev_in, O_RDONLY);
	else if (chev->set.in == 2)
		in = write_in_file(chev->str_chev_in, 0);
	else if (chev->set.in == 3)
		in = write_in_file(chev->str_chev_in, 1);
	dup2(in, STDIN_FILENO);
	close (in);

}

void		redirect_err(t_chev *chev)
{
	int		err;

	chev->save_err = dup(STDERR_FILENO);
	if (chev->set.err == 1)
		err = open(chev->str_chev_err, O_CREAT | O_TRUNC | O_WRONLY);
	else if (chev->set.err == 2)
		err = dup(STDOUT_FILENO);
	else if (chev->set.err == 3)
		err = open(chev->str_chev_err, O_CREAT | O_APPEND | O_WRONLY);
	dup2(err, STDERR_FILENO);
}


void		redirect_b(t_chev *chev)
{
	int		in;

	if (chev->set.out != 0)
	{
		dup2(chev->save_out, STDOUT_FILENO);
		in = open(chev->str_chev_out, O_RDONLY);
	}
	else
		return ;
	dup2(in, STDIN_FILENO);
	close(in);
}

void		redirect_i(t_chev *chev)
{
	if (chev)
		dup2(chev->save_in, STDIN_FILENO);

}

void		redirect_err_b(t_chev *chev)
{
	if (chev)
		dup2(chev->save_err, STDERR_FILENO);

}



void		ft_pipe(t_shell *shell, t_arg *pip)
{
	t_arg	*arg;
	int		pipefd[2];
	int		pid;
	int		fd;

	arg = pip;
	fd = STDIN_FILENO;
	while (arg)
	{
		pipe(pipefd);
		pid = fork();
		if (pid == 0)
		{
			if (arg->prev && arg->prev->chev)
					redirect_i(arg->prev->chev);
			if (arg->prev && arg->prev->chev && arg->prev->chev->str_chev_out)
				redirect_b(arg->prev->chev);
			else
				dup2(fd, STDIN_FILENO);
			if (arg->next)
				dup2(pipefd[1], STDOUT_FILENO);
			close(pipefd[0]);
			if (arg->chev && arg->chev->str_chev_out)
				redirect(arg->chev);
			if (arg->chev && arg->chev->str_chev_in)
				redirect_in(arg->chev);
			if (arg->chev && arg->chev->set.err != 0)
				redirect_err(arg->chev);
			execve(arg->path, arg->argv, shell->envp);
			exit(1);
		}
		else
		{
			wait(NULL);
			close (pipefd[1]);
			fd = pipefd[0];
		}
			arg = arg->next;
	}
}

