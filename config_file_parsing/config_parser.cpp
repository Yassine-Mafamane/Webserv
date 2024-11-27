/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:04:07 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/27 10:17:31 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigTokenizer.hpp"

static void    validate_config(const HttpContext& http_config )
{
    const std::vector<ServerContext> servers = http_config.get_servers();

    if (servers.size() == 0)
    {
        throw std::invalid_argument("Configuration Error: Please specify at least one server within the Http context.");
    }

    std::vector<ServerContext>::const_iterator it = http_config.get_servers().begin();
    std::vector<ServerContext>::const_iterator end = http_config.get_servers().end();

    for (; it < end; it++)
    {
        if (it->get_host() == "")
            throw std::invalid_argument("Configuration Error: Hostname required for server definition but not provided."); 

        if (it->get_root_directory() == "")
            throw std::invalid_argument("Configuration Error: Root directory required for server definition but not provided."); 

        if (it->get_ports().size() == 0)
            throw std::invalid_argument("Configuration Error: Please specify a port for all servers to listen on."); 
    }

}

void    parse_config_file(std::string file_name, HttpContext& http_config)
{
    std::queue<token_info>  tokens_queue;

    tokens_queue = ConfigTokenizer::tokenize(file_name);

    if (tokens_queue.empty())
        throw_config_parse_exception("Empty", "", file_name, 0);

    if (tokens_queue.front().token != "http")
    {
        if (tokens_queue.front().token == ";" || tokens_queue.front().token == "{" || tokens_queue.front().token == "}")
            throw_config_parse_exception("Unexpected", tokens_queue.front().token, file_name, tokens_queue.front().line_num);
        else if (is_http_ctx_dir(tokens_queue.front().token) || is_server_ctx_dir(tokens_queue.front().token) || is_location_ctx_dir(tokens_queue.front().token))
            throw_config_parse_exception("Not allowed", tokens_queue.front().token, file_name, tokens_queue.front().line_num);
        else
            throw_config_parse_exception("Unknown", tokens_queue.front().token, file_name, tokens_queue.front().line_num);
    }
    tokens_queue.pop();

    store_config(http_config, tokens_queue, file_name, "http");

    validate_config(http_config);
}