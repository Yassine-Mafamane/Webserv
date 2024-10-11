/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:09:28 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/11 21:41:30 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_PARSE_HPP
# define CONFIG_PARSE_HPP

#include "HttpContext.hpp"
#include "ServerContext.hpp"
#include "LocationContext.hpp"
#include "../helper_functions.hpp"

#include <iostream>
#include <fstream>
#include <exception>
#include <queue>
#include <string>
#include <sstream>

void    config_tokenizer(std::string file_name);
void    append_token_to_queue(std::string token, std::queue<std::string>& tokens_queue, std::string file_name, unsigned int line_num);
void    store_config(HttpContext& http_config, std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name, std::string context);
bool    is_http_ctx_dir(std::string directive);
bool    is_server_ctx_dir(std::string directive);
bool    is_location_ctx_dir(std::string directive);
void    throw_config_parse_exception(std::string type, std::string token, std::string file_name, unsigned int line_num);
void    throw_wrong_value_exception(std::string directive, std::string wrong_value, std::string file_name, unsigned int line_num);

#endif