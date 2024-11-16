/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   body_parser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 16:59:44 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/16 08:47:35 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request_parse.hpp"

static char hex_to_char(const std::string & hex_code, Request & request)
{
    static const std::string    hex_chars = "0123456789ABCDEF";
	int							value;
    char first = std::toupper(hex_code[0]);
    char second = std::toupper(hex_code[1]);

    if ((first == '0' && second == '0')
        || (hex_chars.find(first) == std::string::npos)
        || (hex_chars.find(second) == std::string::npos)) {
        request.markAsBad(9);
        return 0;
    }

	value = 16 * hex_chars.find(first) + hex_chars.find(second);
	if (value > 127)
	{
		request.markAsBad(9);
        return 0;
	}
    return (value);
}

static bool     contain_unallowed_char(const std::string & field_name)
{
    for (size_t i = 0; i < field_name.length(); i++)
    {
        if ((field_name[i] < 'a' || field_name[i] > 'z')
            && (field_name[i] < 'A' || field_name[i] > 'Z')
            && (field_name[i] != '_')
            && (field_name[i] != '-'))
        {
            return true;
        }
    }
    return false;
}

static void normalize_header_name(std::string & name)
{
    for (size_t i = 0; i < name.length(); i++)
        name[i] = std::toupper(name[i]);
}

static void trim_white_spaces(Request & request, std::string & header_value, const std::string & allowedWS, const std::string & forbidenWS)
{
    for (size_t i = 0; i < header_value.length() && isspace(header_value[i]); i++)
    {
        if (allowedWS.find(header_value[i]) != std::string::npos)
        {
            header_value.erase(i, 1);
            i--;
        }
		else if (forbidenWS.find(header_value[i]) != std::string::npos)
			return request.markAsBad(11);
    }

	for (size_t i = header_value.length(); i > 0 && isspace(header_value[i]); i--)
    {
        if (allowedWS.find(header_value[i]))
        {
            header_value.erase(i, 1);
            i++;
        }
		else if (forbidenWS.find(header_value[i]))
			return request.markAsBad(2);
    }
}

void trim_content_to_boundary_segment(const std::string & boundary, std::string & content)
{
    while (content.length())
    {
        size_t	dash_pos = content.find('-');
        int		i = 0;

        if (dash_pos == std::string::npos)
        {
            content.clear();
            return ;
        }
        content.erase(0, dash_pos);
        while (i != content.length() && content[i] == boundary[i])
            i++;
        
        if (i == content.length())
            return ;
        
        content.erase(0, i);
    }
}

static void	remove_ignored_area(const std::string & boundary, std::string & content)
{
	size_t		boundary_pos;
	std::string	dashed_boundary_crlf = boundary;

	dashed_boundary_crlf.insert(0, "--");
	dashed_boundary_crlf.insert(dashed_boundary_crlf.length(), "\r\n");
	boundary_pos = content.find(dashed_boundary_crlf);

	if (boundary_pos != std::string::npos)
    {
		content.erase(0, boundary_pos);
    }
	else
	{
		content.erase(0, content.length() - dashed_boundary_crlf.length());
		trim_content_to_boundary_segment(dashed_boundary_crlf, content);
	}
}

static std::string	validate_file_name(Request & request, const std::string & value)
{
	std::string                 file_name;
    size_t                      value_length = value.length();

    for (size_t i = 0; i < value_length && !request.isBadRequest() ; i++)
    {
		if (value[i] == '/')
			file_name += '_';
        else if (value[i] != '%')
            file_name += value[i];
        else if (i <= value_length - 3)
        {
            file_name += hex_to_char(value.substr(i + 1, 2), request);
            i += 2;
        }
        else
            request.markAsBad(10);
    }
    return file_name;
}

static void validate_header_value(Request & request, std::string & field_value, std::string & file_name)
{	
	std::stringstream	strm(field_value);
	std::string			param;
	int					i = 0;
	bool				name_param_found = false;

	while (std::getline(strm, param, ';'))
	{
		trim_white_spaces(request, param, " \t\v\f\b", "\n\r");
		if (i == 0 && param != "form-data")
			return ;
		if (param.find("name=") == 0)
		{
			if ((param.length() >= 7) && (param[5] == '"') && (param.back() == '"'))
				name_param_found = true;
			else
				return request.markAsBad(931);
		}
		if (param.find("filename=") == 0)
		{
			if ((param.length() > 11) && (param[9] == '"') && (param.back() == '"'))
				file_name = validate_file_name(request, param.substr(9));
			else
				return request.markAsBad(932);
		}
		i++;
	}
}

static void	check_part_header(Request & request, std::string & header, t_part & part)
{
	std::string field_name;
    std::string field_value;
    size_t      colon_pos;

    colon_pos = header.find(':');
    if (colon_pos == std::string::npos)
        return request.markAsBad(6);

    field_name = header.substr(0, colon_pos);
    if (field_name.empty() || contain_unallowed_char(field_name))
        return request.markAsBad(7);

    normalize_header_name(field_name);

    header.erase(0, colon_pos + 1);
    field_value = header;

	if ((field_name == "CONTENT-DISPOSITION") && (field_value.find("filename=") != std::string::npos))
    	validate_header_value(request , field_value, part.file_name);
}

static void	extract_part_headers(Request & request, t_part & part, std::string & content)
{
	size_t				crlf_pos;
	std::string			header;

	crlf_pos = content.find(CRLF);
    while (crlf_pos != std::string::npos)
	{
		header = content.substr(0, crlf_pos);
		if (header.empty())
		{
			part.header_parsed = true;
			break ;
		}
		if (header.find(request.get_boundary()) != std::string::npos)
			return request.markAsBad(776);
		check_part_header(request, header, part);
		content.erase(0, crlf_pos + 1);
		crlf_pos = content.find(CRLF);
	}
}

static bool	is_valid_condidate_line(const std::string & boundary, std::string & content)
{
	// have to check for the final boundary as well!
	std::string dashed_boundary_crlf = boundary;

	dashed_boundary_crlf.insert(0, CRLF);
	dashed_boundary_crlf.insert(0, "--");
	dashed_boundary_crlf.append(CRLF);

	return (content.find(dashed_boundary_crlf) == 0);
}

static bool	is_possible_condidate_line(const std::string & boundary, std::string & content)
{
	std::string dashed_boundary_crlf = boundary;
	
	dashed_boundary_crlf.insert(0, CRLF);
	dashed_boundary_crlf.insert(0, "--");
	dashed_boundary_crlf.append(CRLF);
	
	return (dashed_boundary_crlf.find(content) == 0)
}

static void	extract_part_content(Request & request, t_part & part, std::string & content)
{
	size_t		crlf_pos;
	std::string	valid_data;

	crlf_pos = content.find(CRLF);
	while (crlf_pos != std::string::npos)
	{
		valid_data.append(content.substr(0, crlf_pos));
		if (valid_data.find(request.get_boundary()) != std::string::npos)
			return request.markAsBad(66);

		part.file_content.append(content.substr(0, crlf_pos));
		content.erase(0, crlf_pos);

		if (is_valid_condidate_line(request.get_boundary(), content))
		{
			content.erase(2);
			part.is_complete = true;
			return ;
		}
		else if (is_possible_condidate_line(request.get_boundary(), content))
		{
			part.unparsed_bytes = content;
			content.clear();
			return ;
		}
		else
			part.file_content.append(CRLF);

		crlf_pos = content.find(CRLF);
	}
	
	
}

static void	extract_part(Request & request, std::string & content)
{
	t_part &			latest_part = request.get_latest_part();
	std::string			dashed_boundary_crlf;

	dashed_boundary_crlf = request.get_boundary();
	dashed_boundary_crlf.insert(0, "--");
	dashed_boundary_crlf.append("\r\n");

	content.insert(0, latest_part.unparsed_bytes);
	latest_part.unparsed_bytes.clear();

	// if it s new that means that in the previous part i found a crlf followed by a dashed_delim_crlf
	if (latest_part.is_new)
	{
		if (!content.find(dashed_boundary_crlf))
		{
			content.erase(0, dashed_boundary_crlf.length());
			latest_part.is_new = false;
		}
		else
		{ // this means that for example we just got a part of the delim and not all of it.
			latest_part.unparsed_bytes = content;
			content.clear();
			return ;
		}
	}

	if (!latest_part.header_parsed)
		extract_part_headers(request, latest_part, content);
	
	if (latest_part.header_parsed)
		extract_part_content(request, latest_part, content);
	else
	{
		latest_part.unparsed_bytes = content;
		content.clear();
	}
		
}

// Processes a single chunk, doesn t matter if the request is chunked or not, if it s not it just assumes that there is only one signle chunk.
static void	process_chunck(Request & request, std::string & content, size_t chunk_length)
{
	std::string	chunk_content;

	if (content.length() > chunk_length)
		chunk_content = content.substr(0, chunk_length);
	else
		chunk_content = content;

	content.erase(0, chunk_content.length());

	if (request.isMultipart())
	{
		if (!request.hasReachedFirstPart())
		{
			remove_ignored_area(request.get_boundary(), chunk_content);
			if (chunk_content.empty())
				return ;
			request.markFirstPartAsReached();
		}
		
		if (!request.hasReachedLastPart())
		{
			while (!chunk_content.empty() && !request.hasReachedLastPart())
			{
				extract_part(request, chunk_content);
			}
		}
		
			
	}
}

void    parse_body(Request & request, std::string & msg )
{

}