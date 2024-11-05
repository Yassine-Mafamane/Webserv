/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request_parser.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:36:24 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/05 22:31:57 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request_parse.hpp"

static char hex_to_char(const std::string& hex) // better dir hadi katreturni lchar value or 0 or throw if it s invalid
{
    static const std::string    valid_hex_chars = "abcdefABCDEF0123456789";
    static const std::string    hex_chars = "0123456789abcdef";
    
    if ((hex[0] == '0' && hex[1] == '0') || (valid_hex_chars.find(hex[0]) == std::string::npos) || (valid_hex_chars.find(hex[1]) == std::string::npos))
        throw "Bad Request";

    return (16 * hex_chars.find(std::tolower(hex[0])) + hex_chars.find(std::tolower(hex[1])));
}

std::string hex_decoder( const std::string& token )
{
    std::string                 decoded_token;
    size_t                      token_length = token.length();

    for (size_t i = 0; i < token.length(); i++)
    {
        if ((token[i] != '%'))
            decoded_token += token[i];
        else if (i <= token_length - 3)
        {
            decoded_token += hex_to_char(token.substr(i + 1, 2));
            i += 2;
        }
        else
            throw "Bad Request";
    }
    return decoded_token;
}

void    extract_url_and_queries(Request& request, std::string uri)
{
    std::string decoded_target;
    size_t  query_start_pos;
    size_t  fragment_start_pos = uri.find('#');

    // Get rid of the fragement part
    if (fragment_start_pos != std::string::npos)
        uri.erase(fragment_start_pos);

    // Setthe ressource location which starts at the begining of the uri and ends at the first '?'
    try
    {
        if ((query_start_pos = uri.find('?')) == std::string::npos)
        {
            decoded_target = hex_decoder(uri);
            request.set_target(decoded_target);
            return ;
        }
        else
            request.set_target(hex_decoder(uri.substr(0, query_start_pos)));
    }
    catch(const char * err)
    {
        request.markAsBad();
        request.set_parsingErrorCode(ERROR_BAD_REQUEST);
        return ;
    }

    request.set_target(hex_decoder(uri));

    uri.erase(0, query_start_pos);
    request.set_query(uri); // does it have to be decoded as well ?
}

static  void    check_method(const std::string& method, Request& request)
{
    request.set_method(method); // check the method components!
}

static void check_uri(const std::string& uri, Request& request)
{
    if (uri[0] != '/')
    {
        request.markAsBad();
        return;
    }
    extract_url_and_queries(request, uri);
}

static void check_version(const std::string& version, Request& request)
{
    if ((version != "HTTP/1.1") && (version != "HTTP/1.0"))
    {
        request.markAsBad();
        return ;
    }
    request.set_version(version);
}

void    parse_start_line(Request& request, std::string start_line)
{
    std::istringstream  line(start_line);
    std::string         token;
    int                 i = 0;
    
    while (std::getline(line, token, ' ') && !request.isBadRequest())
    {
        switch(i)
        {
            case 1:
                check_method(token, request);
                break ;
            case 2:
                check_uri(token, request);
                break ;
            case 3:
                check_version(token, request);
                break ;
            default :
                request.markAsBad();
        }
        i++;
    }

    if (!request.isBadRequest())
        request.markStartLineParsed(true);
}