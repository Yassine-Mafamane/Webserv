/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_line_parser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 18:06:26 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/10 19:14:53 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request_parse.hpp"

static char hex_to_char(const std::string & hex_code, Request & request)
{
    static const std::string    hex_chars = "0123456789ABCDEF";

    char first = std::toupper(hex_code[0]);
    char second = std::toupper(hex_code[1]);

    if ((first == '0' && second == '0')
        || (hex_chars.find(first) == std::string::npos)
        || (hex_chars.find(second) == std::string::npos)) {
        request.markAsBad(9);
        return 0;
    }
    return (16 * hex_chars.find(first) + hex_chars.find(second));
}

static  void    check_method(std::string& method, Request& request)
{
    request.set_method(method); // TODO : Validate characters before setting the request method ? // might contain a white space!
}

void    extract_target(const std::string & uri, Request & request)
{
    std::string                 decoded_target;
    size_t                      target_length = uri.length();

    for (size_t i = 0; i < target_length && !request.isBadRequest() ; i++)
    {
        if ((uri[i] != '%'))
            decoded_target += uri[i];
        else if (i <= target_length - 3)    // Ensure '%' is followed by exactly two characters for valid hex encoding
        {
            decoded_target += hex_to_char(uri.substr(i + 1, 2), request);
            i += 2;
        }
        else
            request.markAsBad(10);
    }
    request.set_target(decoded_target);
}

static void check_uri(std::string & uri, Request & request)
{
    if (uri[0] != '/')
        return request.markAsBad(11);

    size_t  fragment_start_pos = uri.find('#');
    size_t      query_start_pos = uri.find('?');

    // Stripping fragment section (after #) from the URI
    if (fragment_start_pos != std::string::npos)
        uri.erase(fragment_start_pos);

    if (query_start_pos == std::string::npos)
        return extract_target(uri, request);

    extract_target(uri.substr(0, query_start_pos), request);

    // The (extract_target) function might mark the request as bad.
    if (request.isBadRequest())
        return ;

    uri.erase(0, query_start_pos + 1);
    request.set_query(uri); // it may have to be decoded as well !
}

static void check_version(std::string& version, Request& request)
{
    if ((version != "HTTP/1.1") && (version != "HTTP/1.0"))
        return request.markAsBad(12);

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
            case 0:
                check_method(token, request);
                break ;
            case 1:
                check_uri(token, request);
                break ;
            case 2:
                check_version(token, request);
                break ;
            default :
                request.markAsBad(13);
        }
        i++;
    }

    if (!request.isBadRequest())
        request.markStartLineParsed(true);
}