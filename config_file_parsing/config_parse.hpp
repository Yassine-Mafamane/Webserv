/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:09:28 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/10 11:33:56 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSE_HPP
# define CONFIG_PARSE_HPP

#include "HttpContext.hpp"
#include "ServerContext.hpp"
#include "LocationContext.hpp"

#include <iostream>
#include <fstream>
#include <exception>
#include <queue>
#include <string>
#include <sstream>

void    config_tokenizer(std::string file_name);
void    append_token_to_queue(std::string token, std::queue<std::string>& tokens_queue, std::string file_name, unsigned int line_num);

#endif