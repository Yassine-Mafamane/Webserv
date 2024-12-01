/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:07:46 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/30 05:08:39 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "server_setup/Server.hpp"
#include "config_file_parsing/HttpConfigParser.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <netdb.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <queue>
#include <sstream>
#include <fstream>
#include <fcntl.h>

        /* ----------------------------------------------------------------------------------------------------------------- */

/*  Config parsing functions : @ /config_file_parsing/ */

    // Error throwing :

void                                    throw_config_parse_exception(std::string type, std::string token, std::string file_name, unsigned int line_num);
void                                    throw_wrong_value_exception(std::string directive, std::string wrong_value, std::string file_name, unsigned int line_num);

        /* ----------------------------------------------------------------------------------------------------------------- */


#endif