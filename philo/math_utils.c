/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 18:58:31 by fabi              #+#    #+#             */
/*   Updated: 2024/11/11 12:27:57 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_valid_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	parse_number(const char *str, int *i, int *found_digit)
{
	int	res;

	res = 0;
	*found_digit = 0;
	while (str[*i] && (is_valid_digit(str[*i]) || str[*i] == ' '))
	{
		if (is_valid_digit(str[*i]))
		{
			if (res > INT_MAX / 10 || (res == INT_MAX / 10
					&& (str[*i] - '0') > INT_MAX % 10))
			{
				error_exit(MAX);
			}
			res = res * 10 + (str[*i] - '0');
			*found_digit = 1;
		}
		(*i)++;
	}
	return (res);
}

int	ft_atoi(const char *str)
{
	int		i;
	int		found_digit;
	int		res;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			error_exit(NEGATIVE);
		i++;
	}
	res = parse_number(str, &i, &found_digit);
	if (!found_digit)
		error_exit(NAN);
	if (str[i] != '\0')
		error_exit(WRONG_ARGV);
	return (res);
}
