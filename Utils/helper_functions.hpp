/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 21:39:03 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/29 17:40:20 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_FUNCTIONS_HPP
# define HELPER_FUNCTIONS_HPP

#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "../Server Setup/Server.hpp"

bool is_all_digits(const std::string& str);
bool    is_space(char c);
void	*ft_memset(void *b, int c, size_t len);
void    err_throw( const char * message );
void    close_sockets_on_error(std::vector<struct SocketListener>& active_socks);

#endif