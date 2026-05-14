#include "codexion.h"

int	main(int ac, char **av)
{
	t_sim	sim;

	memset(&sim, 0, sizeof(sim));
	if (parse_args(ac, av, &sim.cfg) != 0)
	{
		fprintf(stderr, "Usage: %s n_coders t_burnout t_compile t_debug "
			"t_refactor n_compiles_required dongle_cooldown {fifo|edf}\n",
			av[0]);
		return (1);
	}
	if (sim_init(&sim) != 0)
	{
		sim_destroy(&sim);
		return (1);
	}
	if (sim_start(&sim) != 0)
	{
		sim_destroy(&sim);
		return (1);
	}
	sim_destroy(&sim);
	return (0);
}
