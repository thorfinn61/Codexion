#include "codexion.h"

static int	parse_positive(const char *s, long *out, int allow_zero)
{
	if (safe_atol(s, out) != 0)
		return (-1);
	if (*out < 0)
		return (-1);
	if (!allow_zero && *out == 0)
		return (-1);
	return (0);
}

static int	parse_scheduler(const char *s, int *out)
{
	if (strcmp(s, "fifo") == 0)
		*out = SCHED_FIFO_POLICY;
	else if (strcmp(s, "edf") == 0)
		*out = SCHED_EDF_POLICY;
	else
		return (-1);
	return (0);
}

int	parse_args(int ac, char **av, t_config *cfg)
{
	long	n;

	if (ac != 9)
		return (-1);
	if (parse_positive(av[1], &n, 0) != 0)
		return (-1);
	cfg->n_coders = (int)n;
	if (parse_positive(av[2], &cfg->t_burnout, 0) != 0)
		return (-1);
	if (parse_positive(av[3], &cfg->t_compile, 0) != 0)
		return (-1);
	if (parse_positive(av[4], &cfg->t_debug, 0) != 0)
		return (-1);
	if (parse_positive(av[5], &cfg->t_refactor, 0) != 0)
		return (-1);
	if (parse_positive(av[6], &n, 0) != 0)
		return (-1);
	cfg->n_compiles_required = (int)n;
	if (parse_positive(av[7], &cfg->dongle_cooldown, 1) != 0)
		return (-1);
	if (parse_scheduler(av[8], &cfg->scheduler) != 0)
		return (-1);
	return (0);
}
