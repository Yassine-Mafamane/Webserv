/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_storing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:47:39 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/12 19:59:43 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parse.hpp"

void    store_http_directives(HttpContext& http_config, std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    /* The following flags are used to track duplicate directives */
    static bool    cgi_ext_is_set;
    static bool    auto_ind_is_set;

    std::string token = tokens_queue.front().second;

    if (!is_http_ctx_dir(token))
    {
        // TODO : The following lines are repeated multiple times, create a function to do this repetetive job;
        if (token == ";" || token == "{" || token == "}")
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
        else if (is_server_ctx_dir(token) || is_location_ctx_dir(token))
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
        if (auto_ind_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);

        http_config.set_auto_index(extract_autoindex_value(tokens_queue, file_name));

        auto_ind_is_set = true;
    }
    else if (token == "error_page")
    {
        http_config.set_error_page(extract_error_page_info(tokens_queue, file_name));
    }
    else if (token == "cgi_extention")
    {
        if (cgi_ext_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);

        http_config.set_cgi_extension(extract_cgi_extension(tokens_queue, file_name));

        cgi_ext_is_set = true;
    }
}

void    store_serv_directives(HttpContext& http_config, std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    ServerContext   current_serv_ctx = http_config.get_latest_server();
    static bool     srv_names_is_set;
    static bool     auto_ind_is_set;
    static bool     cgi_ext_is_set;
    static bool     methods_is_set;
    static bool     upl_dir_is_set;
    static bool     index_is_set;
    static bool     port_is_set;
    static bool     root_is_set;

    std::string token = tokens_queue.front().second;

    if (!is_server_ctx_dir(token))
    {
        if (token == ";" || token == "{" || token == "}")
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
        else if (is_http_ctx_dir(token) || is_location_ctx_dir(token))
            throw_config_parse_exception("Not allowed", token, file_name, tokens_queue.front().first);
        else
            throw_config_parse_exception("Unknown", token, file_name, tokens_queue.front().first);
    }
    else if (token == "location")
    {
        tokens_queue.pop();
        store_config(http_config, tokens_queue, file_name, "location");
    }
    else if (token == "autoindex")
    {
        if (auto_ind_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);

        current_serv_ctx.set_auto_index(extract_autoindex_value(tokens_queue, file_name));

        auto_ind_is_set = true;
    }
    else if (token == "error_page")
    {
        current_serv_ctx.set_error_page(extract_error_page_info(tokens_queue, file_name));
    }
    else if (token == "cgi_extention")
    {
        if (cgi_ext_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);

        current_serv_ctx.set_cgi_extension(extract_cgi_extension(tokens_queue, file_name));

        cgi_ext_is_set = true;
    }
    else if (token == "listen")
    {
        if (port_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);
        
        current_serv_ctx.set_port(extract_port_number(tokens_queue, file_name));

        port_is_set = true;
    }
    else if (token == "root")
    {
        if (port_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);
        
        current_serv_ctx.set_root_directory(extract_root_dir(tokens_queue, file_name));

        root_is_set = true;
    }
    else if (token == "upload_dir")
    {
        if (upl_dir_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);
        
        current_serv_ctx.set_upload_dir(extract_upload_dir(tokens_queue, file_name));

        upl_dir_is_set = true;
    }
    else if (token == "index")
    {
        if (index_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);
        
        current_serv_ctx.set_upload_dir(extract_index(tokens_queue, file_name));

        index_is_set = true;
    }
    else if (token == "server_names")
    {
        if (srv_names_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);
        
        current_serv_ctx.set_server_names(extract_srv_names(tokens_queue, file_name));

        srv_names_is_set = true;
    }
    else if (token == "allowed_methods")
    {
        if (methods_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().first);

        current_serv_ctx.set_allowed_methods(extract_allowed_methods(tokens_queue, file_name));

        methods_is_set = true;
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
            http_config.set_new_server();
            store_serv_directives(http_config, tokens_queue, file_name);
            return ;
        }
        else if (context == "location")
        {
            // ...
            return ;
        }
    }

    if (tokens_queue.empty())
    {
        throw_config_parse_exception("EOF", "", file_name, tokens_queue.front().first);
    }

    // check that nothing is left in the queue
}