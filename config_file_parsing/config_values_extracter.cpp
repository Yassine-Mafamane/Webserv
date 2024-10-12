/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_values_extracter.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 15:44:35 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/12 19:54:02 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parse.hpp"

bool    extract_autoindex_value(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    std::string token;
    bool        value;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
    else if (token != "on" && token != "off")
        throw_wrong_value_exception("autoindex", token, file_name, tokens_queue.front().first);
    else if (token == "on")
        value = true;
    else
        value = false;

    tokens_queue.pop();
    token = tokens_queue.front().second;
    
    if (token == ";")
    {
        tokens_queue.pop();
        return value;
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "autoindex", file_name, tokens_queue.front().first);
    else
        throw_wrong_value_exception("wrong args num", "autoindex", file_name, tokens_queue.front().first);
    
    return value; // this line is added to silence the warnings at compilation.
}

std::pair <unsigned short, std::string> extract_error_page_info(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    unsigned short  error_code;
    std::string     token;
    std::string     path;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
    else if (!is_all_digits(token) || token.length() != 3 || token < "300" || token > "599")
        throw_wrong_value_exception("error_page", token, file_name, tokens_queue.front().first);
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

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";")
    {
        tokens_queue.pop();
        return std::make_pair(error_code, path);
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "autoindex", file_name, tokens_queue.front().first);
    else
        throw_wrong_value_exception("wrong args num", "autoindex", file_name, tokens_queue.front().first);
    
    return std::make_pair(error_code, token);  // this line is added to silence the warnings at compilation.
}

std::string extract_cgi_extension(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    std::string cgi_extension;
    std::string token;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
    else if (token != ".php")
        throw_wrong_value_exception("cgi_extension", token, file_name, tokens_queue.front().first);
    else
        cgi_extension = token;
    
    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";")
    {
        tokens_queue.pop();
        return cgi_extension;
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "cgi_extention", file_name, tokens_queue.front().first);
    else
        throw_wrong_value_exception("wrong args num", "cgi_extention", file_name, tokens_queue.front().first);
    
    return cgi_extension;  // this line is added to silence the warnings at compilation.
}

unsigned short  extract_port_number(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    unsigned short  port_num;
    std::string     token;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
    else if (!is_all_digits(token) || (token.length() != 4 && token.length() != 5) || token < "1024" || token > "49151")
        throw_wrong_value_exception("listen", token, file_name, tokens_queue.front().first);
    else
        port_num = std::stoi(token);
    
    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";")
    {
        tokens_queue.pop();
        return port_num;
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "listen", file_name, tokens_queue.front().first);
    else
        throw_wrong_value_exception("wrong args num", "listen", file_name, tokens_queue.front().first);
    
    return port_num;  // this line is added to silence the warnings at compilation.
}

std::string    extract_root_dir(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    std::string root;
    std::string token;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
    else
        root = token;
    
    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";")
    {
        tokens_queue.pop();
        return root;
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "root", file_name, tokens_queue.front().first);
    else
        throw_wrong_value_exception("wrong args num", "root", file_name, tokens_queue.front().first);
    
    return root;  // this line is added to silence the warnings at compilation.
}

std::string extract_upload_dir(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    std::string directory;
    std::string token;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
    else
        directory = token;
    
    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";")
    {
        tokens_queue.pop();
        return directory;
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "upload_dir", file_name, tokens_queue.front().first);
    else
        throw_wrong_value_exception("wrong args num", "upload_dir", file_name, tokens_queue.front().first);
    
    return directory;  // this line is added to silence the warnings at compilation.
}

std::string extract_index(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    std::string index;
    std::string token;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);
    else
        index = token;
    
    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";")
    {
        tokens_queue.pop();
        return index;
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "index", file_name, tokens_queue.front().first);
    else
        throw_wrong_value_exception("wrong args num", "index", file_name, tokens_queue.front().first);
    
    return index;  // this line is added to silence the warnings at compilation.
}

std::vector<std::string>    extract_srv_names(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    std::vector<std::string>    names;
    std::string                 token;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);

    while (token != ";" && token != "{" && token != "}")
    {
        names.push_back(token);

        tokens_queue.pop();
        token = tokens_queue.front().second;
    }

    if (token == ";")
    {
        tokens_queue.pop();
        return names;
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "server_names", file_name, tokens_queue.front().first);

    return names; // this line is added to silence the warnings at compilation.
}

std::vector<std::string>    extract_allowed_methods(std::queue<std::pair<unsigned int, std::string> >& tokens_queue, std::string file_name)
{
    std::vector<std::string>    methods;
    std::string                 token;

    tokens_queue.pop();
    token = tokens_queue.front().second;

    if (token == ";" || token == "{" || token == "}")
        throw_config_parse_exception("Unexpected", token, file_name, tokens_queue.front().first);

    while (token != ";" && token != "{" && token != "}")
    {
        if (token != "GET" && token != "POST" && token != "DELETE" && token != "HEAD")
            throw_wrong_value_exception("allowed_methods", token, file_name, tokens_queue.front().first);
        else
        {
            methods.push_back(token);
            tokens_queue.pop();
            token = tokens_queue.front().second;
        }
    }

    if (token == ";")
    {
        tokens_queue.pop();
        return methods;
    }
    else if (token == "{" || token == "}")
        throw_config_parse_exception("unterminated", "allowed_methods", file_name, tokens_queue.front().first);

    return methods; // this line is added to silence the warnings at compilation.
}