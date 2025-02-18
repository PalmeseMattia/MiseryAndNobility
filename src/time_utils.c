/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpalmese <dpalmese@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:39:30 by dpalmese          #+#    #+#             */
/*   Updated: 2024/10/11 14:44:38 by dpalmese         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/philosophers.h"

long long	get_milliseconds(void)
{
	struct timeval	tv;
	int				time_res;

	time_res = gettimeofday(&tv, NULL);
	if (time_res != 0)
	{
		printf("Error getting time of the day!\n");
		exit(EXIT_FAILURE);
	}
	else
		return ((((long long)tv.tv_sec) * 1000) + (tv.tv_usec / 1000));
}
