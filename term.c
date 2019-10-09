#include "minishell.h"


void		error(char *s)
{
	ft_putstr("error: ");
	ft_putendl(s);
	exit (1);

}


void init_term(t_shell *shell)
{
	if (!isatty(0))
		error("isatty");
	if ((shell->fd = open(ttyname(0), O_RDWR)) == -1)
		error("tyyname");
	if (tgetent(NULL, getenv("TERM")) < 1)
		error("tgetent");
	if (tcgetattr(shell->fd, &(shell->termios)) == -1)
		error("tcgetattr");
	shell->termios.c_lflag &= ~(ICANON);
	shell->termios.c_lflag &= ~(ECHO);
	if (tcsetattr(shell->fd, TCSANOW, &(shell->termios)) == -1)
		error("tcsetattr");
	tputs(tgetstr("vi", NULL), 1, ft_poutchar);
	tputs(tgetstr("cl", NULL), 1, ft_poutchar);
}

void		update_term(t_shell *shell)
{
	if (!isatty(0))
		error("isatty");
	if ((shell->fd = open(ttyname(0), O_RDWR)) == -1)
		error("tyyname");
	if (tgetent(NULL, getenv("TERM")) < 1)
		error("tgetent");
	if (tcgetattr(shell->fd, &(shell->termios)) == -1)
		error("tcgetattr");
	shell->termios.c_lflag &= ~(ICANON);
	shell->termios.c_lflag &= ~(ECHO);
	if (tcsetattr(shell->fd, TCSANOW, &(shell->termios)) == -1)
		error("tcsetattr");
	tputs(tgetstr("vi", NULL), 1, ft_poutchar);


}


void		process_term(t_shell *shell)
{
	if (tcgetattr(shell->fd, &(shell->termios)) == -1)
		error("tcgetattr");
	shell->termios.c_lflag |= (ICANON | ECHO);
	shell->termios.c_cc[VEOF] = 4;
	if (tcsetattr(shell->fd, TCSADRAIN, &(shell->termios)) == -1)
		error("tcsetattr");
	tputs(tgetstr("ve", NULL),1, ft_poutchar);

}

void		show_curs(void)
{
	tputs(tgetstr("ve", NULL), 1, ft_poutchar);
}

void		del_curs(void)
{
	tputs(tgetstr("vi", NULL), 1, ft_poutchar);

}

void		exit_term(t_shell *shell)
{
	if (tcgetattr(shell->fd, &(shell->termios)) == -1)
		error("tcgetattr");
	shell->termios.c_lflag |= (ICANON | ECHO);
	if (tcsetattr(shell->fd, TCSANOW, &(shell->termios)) == -1)
		error("tcsetattr");
	tputs(tgetstr("ve", NULL),1, ft_poutchar);
	tputs(tgetstr("te", NULL), 1, ft_poutchar);
	exit(1);
}


