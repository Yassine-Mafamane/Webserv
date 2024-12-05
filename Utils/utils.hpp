/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 21:45:38 by ymafaman          #+#    #+#             */
/*   Updated: 2024/12/04 02:20:59 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include <string>

bool    is_space(char c);
void    err_throw( const char * message );
bool    is_all_digits(const std::string& str);
void	*ft_memset(void *b, int c, size_t len);

#endif