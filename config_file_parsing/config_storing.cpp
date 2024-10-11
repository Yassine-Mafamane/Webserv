/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_storing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:47:39 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/11 22:15:44 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parse.hpp"

void    store_http_directives(HttpContext& http_config, std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    /* The following flags are used to track duplicate directives */
    bool    cgi_extension_set = false;
    bool    auto_index_set = false;


    std::string token = tokens_queue.front().second;

    if (!is_http_ctx_dir(token))
    {
        // TODO : The following lines are repeated multiple times, create a function to do this repetetive job;
        if (token == ";" || token == "{" || token == "}")
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
        else if (is_http_ctx_dir(token) || is_server_ctx_dir(token) || is_location_ctx_dir(token))
            throw_config_parse_exception("Not allowed", token, file_name, tokens_queue.front().first);
        else
            throw_config_parse_exception("Unknown", token, file_name, tokens_queue.front().first); 
    }
    else if (token == "server")
    {
        tokens_queue.pop();
        store_config(http_config, tokens_queue, file_name, "server");
    }
    else if (token == "autoindex")
    {
        if (auto_index_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);

        tokens_queue.pop();
        token = tokens_queue.front().second;

        if (token == ";" || token == "{" || token == "}")
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
        else if (token != "on" && token != "off")
            throw_wrong_value_exception("autoindex", token, file_name, tokens_queue.front().first);
        else
            http_config.set_auto_index(true);

        auto_index_set = true;

        tokens_queue.pop();
        token = tokens_queue.front().second;
        
        if (token == ";")
            return ;
        else if (token == "{" || token == "}")
            throw_config_parse_exception("unterminated", "autoindex", file_name, tokens_queue.front().first);
        else
            throw_wrong_value_exception("wrong args num", "autoindex", file_name, tokens_queue.front().first);
        
        tokens_queue.pop();
    }
    else if (token == "error_page")
    {
        unsigned short  error_code;
        std::string     path;

        tokens_queue.pop();
        token = tokens_queue.front().second;

        if (token == ";" || token == "{" || token == "}")
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
        else if (!is_all_digits(token) || token < "300" || token > "599")
            throw_wrong_value_exception("error_page", tokens_queue.front().second, file_name, tokens_queue.front().first);
        else
            error_code = std::stoi(token);
        
        tokens_queue.pop();
        token = tokens_queue.front().second;

        if (token == ";")
            throw_wrong_value_exception("wrong args num", "error_page", file_name, tokens_queue.front().first);
        else if (token == "{" || token == "}")
            throw_config_parse_exception("unterminated", "error_page", file_name, tokens_queue.front().first);
        else
            path = token;
        
        http_config.set_error_page(std::make_pair(error_code, token));

        tokens_queue.pop();
        token = tokens_queue.front().second;

        if (token == ";")
            return ;
        else if (token == "{" || token == "}")
            throw_config_parse_exception("unterminated", "autoindex", file_name, tokens_queue.front().first);
        else
            throw_wrong_value_exception("wrong args num", "autoindex", file_name, tokens_queue.front().first);
        
        tokens_queue.pop();
    }
    else if (token == "cgi_extention")
    {
        tokens_queue.pop();
        token = tokens_queue.front().second;

        if (token == ";" || token == "{" || token == "}")
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
        else if (token != ".php")
            throw_wrong_value_exception("cgi_extention", tokens_queue.front().second, file_name, tokens_queue.front().first);
        else
            http_config.set_cgi_extension(token);
        
        tokens_queue.pop();
        token = tokens_queue.front().second;

        if (token == ";")
            return ;
        else if (token == "{" || token == "}")
            throw_config_parse_exception("unterminated", "cgi_extention", file_name, tokens_queue.front().first);
        else
            throw_wrong_value_exception("wrong args num", "cgi_extention", file_name, tokens_queue.front().first);
    }
}

void    store_config(HttpContext& http_config, std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name, std::string context)
{
    if (context == "location" && tokens_queue.front().second == "=")
    {
        /* Have to declare that it s an exact location */
        tokens_queue.pop();
    }

    if (tokens_queue.front().second != "{")
    {
        throw_config_parse_exception("Unexpected", tokens_queue.front().second, file_name, tokens_queue.front().first);
    }
    
    tokens_queue.pop();
    while (!tokens_queue.empty() && tokens_queue.front().second != "}")
    {
        /* Here i will be extracting directives and store them one by one */
        if (context == "http")
        {
            store_http_directives(http_config, tokens_queue, file_name);
        }
        else if (context == "server")
        {
            
        }
        else if (context == "location")
        {
            
        }
    }

    if (tokens_queue.empty())
    {
        throw_config_parse_exception("EOF", "", file_name, tokens_queue.front().first);
    }
}