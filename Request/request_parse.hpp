/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parse.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:47:07 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/08 17:02:21 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_PARSE_HPP
# define REQUEST_PARSE_HPP

#include <string>
#include "Request.hpp"
#include "../macros.hpp"

void    parse_start_line(Request& request, std::string start_line);
void    parse_headers(Request & request, std::string & msg);
void    parse_body(Request & request, std::string & msg );

#endif