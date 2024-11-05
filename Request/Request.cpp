/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:06:28 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/05 21:47:41 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void ) : start_line_is_parsed(false), is_bad(false)
{
    
}

Request::~Request()
{
    
}

/* Getters */

bool    Request::hasParsedStartLine()
{
    return this->start_line_is_parsed;
}


bool    Request::isBadRequest()
{
    return this->is_bad;
}

std::string Request::get_target()
{
    return this->target;
}

/* Setters */

void    Request::set_method( const std::string& method )
{
    this->method = method;
}

void    Request::set_version( const std::string& version )
{
    this->version = version;
}

void    Request::set_target( const std::string& target )
{
    this->target = target;
}

void    Request::set_query( std::string query )
{
    this->query = query;
}

void    Request::markStartLineParsed( const bool& parsed )
{
    this->start_line_is_parsed = parsed;
}

void    Request::markAsBad()
{
    this->is_bad = true;
}

void    Request::set_parsingErrorCode( short code )
{
    this->parsingErrorCode = code;
}