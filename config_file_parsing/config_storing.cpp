/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_storing.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/11 14:47:39 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/13 21:50:19 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parse.hpp"

void    store_http_directives(HttpContext& http_config, std::queue<token_info>& tokens_queue, std::string file_name)
{
    /* The following flags are used to track duplicate directives */
    static bool    cgi_ext_is_set;
    static bool    auto_ind_is_set;

    std::string token = tokens_queue.front().token;
    std::cout << "-->" << token << std::endl;
    if (!is_http_ctx_dir(token))
    {
        // TODO : The following lines are repeated multiple times, create a function to do this repetetive job;
        if ((token == ";" || token == "{" || token == "}") && tokens_queue.front().is_sep)
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().line_num);
        else if (is_server_ctx_dir(token) || is_location_ctx_dir(token))
            throw_config_parse_exception("Not allowed", token, file_name, tokens_queue.front().line_num);
        else
            throw_config_parse_exception("Unknown", token, file_name, tokens_queue.front().line_num); 
    }
    else if (token == "server")
    {
            std::cout << "gg" << std::endl;

        tokens_queue.pop();
        store_config(http_config, tokens_queue, file_name, "server");
    }
    else if (token == "autoindex")
    {
        if (auto_ind_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);

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
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);

        http_config.set_cgi_extension(extract_cgi_extension(tokens_queue, file_name));

        cgi_ext_is_set = true;
    }
}

void    store_serv_directives(HttpContext& http_config, std::queue<token_info>& tokens_queue, std::string file_name)
{
    ServerContext   server = http_config.get_latest_server();

    static bool     srv_names_is_set;
    static bool     auto_ind_is_set;
    static bool     cgi_ext_is_set;
    static bool     methods_is_set;
    static bool     upl_dir_is_set;
    static bool     index_is_set;
    static bool     port_is_set;
    static bool     root_is_set;

    std::string token = tokens_queue.front().token;

    if (!is_server_ctx_dir(token))
    {
        if ((token == ";" || token == "{" || token == "}") && tokens_queue.front().is_sep)
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().line_num);
        else if (is_http_ctx_dir(token) || is_location_ctx_dir(token))
            throw_config_parse_exception("Not allowed", token, file_name, tokens_queue.front().line_num);
        else
            throw_config_parse_exception("Unknown", token, file_name, tokens_queue.front().line_num);
    }
    else if (token == "location")
    {
        /* Creating a new locationContext object in the ServerLocation::locations vector */
        http_config.get_latest_server().set_new_location();

        tokens_queue.pop();
        token = tokens_queue.front().token;

        /* Cheking if the location is exact or not */
        if (token == "=")
        {
            http_config.get_latest_server().get_latest_location().make_exact();
            tokens_queue.pop();
        }

        /* Getting the location's relative path */
        token = tokens_queue.front().token;

        if ((token == ";" || token == "{" || token == "}") && tokens_queue.front().is_sep)
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().line_num);
        else
            http_config.get_latest_server().get_latest_location().set_location(token);

        tokens_queue.pop(); // now the current token should be a "{";

        store_config(http_config, tokens_queue, file_name, "location");
    }
    else if (token == "autoindex")
    {
        if (auto_ind_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);

        server.set_auto_index(extract_autoindex_value(tokens_queue, file_name));

        auto_ind_is_set = true;
    }
    else if (token == "error_page")
    {
        server.set_error_page(extract_error_page_info(tokens_queue, file_name));
    }
    else if (token == "cgi_extention")
    {
        if (cgi_ext_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);

        server.set_cgi_extension(extract_cgi_extension(tokens_queue, file_name));

        cgi_ext_is_set = true;
    }
    else if (token == "listen")
    {
        if (port_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        server.set_port(extract_port_number(tokens_queue, file_name));

        port_is_set = true;
    }
    else if (token == "root")
    {
        if (root_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        server.set_root_directory(extract_root_dir(tokens_queue, file_name));

        root_is_set = true;
    }
    else if (token == "upload_dir")
    {
        if (upl_dir_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        server.set_upload_dir(extract_upload_dir(tokens_queue, file_name));

        upl_dir_is_set = true;
    }
    else if (token == "index")
    {
        if (index_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        server.set_upload_dir(extract_index(tokens_queue, file_name));

        index_is_set = true;
    }
    else if (token == "server_names")
    {
        if (srv_names_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        server.set_server_names(extract_srv_names(tokens_queue, file_name));

        srv_names_is_set = true;
    }
    else if (token == "allowed_methods")
    {
        if (methods_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);

        server.set_allowed_methods(extract_allowed_methods(tokens_queue, file_name));

        methods_is_set = true;
    }
}

void    store_location_directives(LocationContext& location, std::queue<token_info>& tokens_queue, std::string file_name)
{
    static bool     redirect_is_set;
    static bool     auto_ind_is_set;
    static bool     cgi_ext_is_set;
    static bool     methods_is_set;
    static bool     upl_dir_is_set;
    static bool     index_is_set;
    static bool     root_is_set;

    std::string token = tokens_queue.front().token;

    

    if (!is_location_ctx_dir(token))
    {
        if ((token == ";" || token == "{" || token == "}") && tokens_queue.front().is_sep)
            throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().line_num);
        else if (is_http_ctx_dir(token) || is_location_ctx_dir(token))
            throw_config_parse_exception("Not allowed", token, file_name, tokens_queue.front().line_num);
        else
            throw_config_parse_exception("Unknown", token, file_name, tokens_queue.front().line_num);
    }
    else if (token == "autoindex")
    {
        if (auto_ind_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);

        location.set_auto_index(extract_autoindex_value(tokens_queue, file_name));

        auto_ind_is_set = true;
    }
    else if (token == "error_page")
    {
        location.set_error_page(extract_error_page_info(tokens_queue, file_name));
    }
    else if (token == "cgi_extention")
    {
        if (cgi_ext_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);

        location.set_cgi_extension(extract_cgi_extension(tokens_queue, file_name));

        cgi_ext_is_set = true;
    }
     else if (token == "root")
    {
        if (root_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        location.set_root_directory(extract_root_dir(tokens_queue, file_name));

        root_is_set = true;
    }
    else if (token == "upload_dir")
    {
        if (upl_dir_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        location.set_upload_dir(extract_upload_dir(tokens_queue, file_name));

        upl_dir_is_set = true;
    }
    else if (token == "index")
    {
        if (index_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        location.set_upload_dir(extract_index(tokens_queue, file_name));

        index_is_set = true;
    }
    else if (token == "allowed_methods")
    {
        if (methods_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);

        location.set_allowed_methods(extract_allowed_methods(tokens_queue, file_name));

        methods_is_set = true;
    }
    else if (token == "return")
    {
        if (redirect_is_set)
            throw_config_parse_exception("duplication", token, file_name, tokens_queue.front().line_num);
        
        location.set_redirection(extract_redirection_info(tokens_queue, file_name));

        redirect_is_set = true;
    }
    
}

void    store_config(HttpContext& http_config, std::queue<token_info>& tokens_queue, std::string file_name, std::string context)
{
    bool    is_inside_serv = false;
    
    std::cout << "Context: " << context << std::endl;

    if (tokens_queue.front().token != "{")
    {
        throw_config_parse_exception("no_openning", context, file_name, tokens_queue.front().line_num);
    }

    tokens_queue.pop();

    while (!tokens_queue.empty() && tokens_queue.front().token != "}")
    {
        /* Here i will be extracting directives and store them one by one */
        if (context == "http")
        {
            is_inside_serv = false;

            store_http_directives(http_config, tokens_queue, file_name);
        }
        else if (context == "server")
        {
            if (!is_inside_serv)
                http_config.set_new_server();
            is_inside_serv = true;
            store_serv_directives(http_config, tokens_queue, file_name);
        }
        else if (context == "location")
        {
            store_location_directives(http_config.get_latest_server().get_latest_location(), tokens_queue, file_name);
        }
    }

    if (tokens_queue.empty())
    {
        throw_config_parse_exception("unclosed_ctx", "", file_name, tokens_queue.front().line_num);
    }

    tokens_queue.pop();

    if (context == "http" && !tokens_queue.empty())
    {
        throw_config_parse_exception("Unexpected", tokens_queue.front().token, file_name, tokens_queue.front().line_num);
    }
    
}