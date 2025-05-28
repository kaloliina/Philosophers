/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_and_exits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 14:37:58 by khiidenh          #+#    #+#             */
/*   Updated: 2025/05/28 17:47:52 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	clean_up(t_table *table, enum e_fail_stages fail_stage, int fork_amount)
{
	int	i;

	if (table == NULL)
		return ;
	i = 0;
	if (fail_stage != MALLOC)
	{
		while (i < fork_amount)
		{
			pthread_mutex_destroy(&table->forks[i]);
			i++;
		}
		if (fail_stage != PRINT_LOCK)
			pthread_mutex_destroy(&table->print_lock);
		if (fail_stage != PRINT_LOCK && fail_stage != MEAL_LOCK)
			pthread_mutex_destroy(&table->meal_check_lock);
		if (fail_stage != PRINT_LOCK && fail_stage != MEAL_LOCK && fail_stage != FINISHED_LOCK)
			pthread_mutex_destroy(&table->finished_lock);
	}
	if (table->forks != NULL)
		free (table->forks);
	if (table->philos != NULL)
		free (table->philos);
	free (table);
}
