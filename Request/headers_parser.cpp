/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   headers_parser.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 19:05:24 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/10 20:01:23 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request_parse.hpp"

// TODO : Limit the maximum header possible length.

static bool contain_white_space(std::string token)
{
    for (size_t i = 0; i < token.length(); i++)
    {
        if (isspace(token[i]))
            return true;
    }
    return false;
}

static void	validate_host(Request & request, std::string & host_value)
{

	size_t		colon_pos = host_value.find(':'); // The colon position is the start of the PORT part in the HOST:PORT or SERVER_NAME:PORT combo.

    if (colon_pos != std::string::npos)
        host_value.erase(0, colon_pos + 1);

    // better check if it s duplicated first
	for (size_t i = 0; i < host_value.length() && isspace(host_value[i]); i++)
    {
        if (host_value[i] == ' ')
        {
            host_value.erase(i, 1);
            i--;
        }
		else if (isspace(host_value[i]))
			return request.markAsBad(1);
    }

	for (size_t i = host_value.length(); i > 0 && isspace(host_value[i]); i--)
    {
        if (host_value[i] == ' ')
        {
            host_value.erase(i, 1);
            i++;
        }
		else if (isspace(host_value[i]))
			return request.markAsBad(2);
    }
	if (host_value.empty() || contain_white_space(host_value))
		return request.markAsBad(3);

	return request.setHeader("HOST", host_value);
}

static void trim_spaces(Request & request, std::string & field_value)
{
    for (size_t i = 0; i < field_value.length() && isspace(field_value[i]); i++)
    {
        if (field_value[i] == ' ' || field_value[i] == '\t' || field_value[i] == '\v' || field_value[i] == '\f')
        {
            field_value.erase(i, 1);
            i--;
        }
		else if (field_value[i] == '\n' || field_value[i] == '\r')
			return request.markAsBad(4);
    }

	for (size_t i = field_value.length(); i > 0 && isspace(field_value[i]); i--)
    {
        if (field_value[i] == ' ' || field_value[i] == '\t' || field_value[i] == '\v' || field_value[i] == '\f')
        {
            field_value.erase(i, 1);
            i++;
        }
		else if (field_value[i] == '\n' || field_value[i] == '\r')
			return request.markAsBad(5);
    }
}

static void normalize_header_name(std::string & name)
{
    for (size_t i = 0; i < name.length(); i++)
        name[i] = std::toupper(name[i]);
}

static void validate_header_value(Request & request, std::string & field_name, std::string & field_value)
{
    if (field_name == "HOST")
        return validate_host(request, field_value);

	trim_spaces(request, field_value);
    // Check content length validity
	return request.setHeader(field_name, field_value);
}

static void    parse_header(Request & request, std::string & header)
{
    std::string field_name;
    std::string field_value;
    size_t      colon_pos;

    colon_pos = header.find(':');
    if (colon_pos == std::string::npos)
    {
        return request.markAsBad(6);
    }

    field_name = header.substr(0, colon_pos);
    if (field_name.empty() || contain_white_space(field_name))
        return request.markAsBad(7);

    normalize_header_name(field_name);

    header.erase(0, colon_pos + 1);
    field_value = header;

    validate_header_value(request , field_name, field_value);
}

void    parse_headers(Request & request, std::string & msg)
{
    std::string         line;
    size_t              crlf_pos;

    crlf_pos = msg.find(CRLF);
    while ((crlf_pos != std::string::npos) && !request.isBadRequest())
    {
    	line = msg.substr(0, crlf_pos);
        if (line.empty())
        {
            request.markHeadersParsed(true);
            msg.erase(0, 2);
            break ;
        }
        parse_header(request, line);
		msg.erase(0, crlf_pos + 2);
    	crlf_pos = msg.find(CRLF);
    }
}
