#include "minishell.h"


void		exit_builtin(t_shell *shell)
{
	free(shell);
	exit(0);

}
