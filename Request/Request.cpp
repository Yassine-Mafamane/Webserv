/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 11:06:28 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/09 17:00:52 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request( void ) : start_line_is_parsed(false), headers_parsed(false), body_is_parsed(false), is_bad(false)
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

bool    Request::hasParsedHeaders()
{
    return this->headers_parsed;
}

bool    Request::hasParsedBody()
{
    return this->body_is_parsed;
}

bool    Request::isBadRequest()
{
    return this->is_bad;
}

bool    Request::isReady()
{
    return this->is_ready;
}


std::string Request::get_target()
{
    return this->target;
}

std::string Request::get_query()
{
    return this->query;
}

std::string Request::get_method()
{
    return this->method;
}

std::string Request::get_version()
{
    return this->version;
}

std::string Request::get_body()
{
    return this->body;
}

std::string Request::getUnparsedMsg()
{
    return this->unparsed_msg;
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

void    Request::markHeadersParsed( const bool & parsed )
{
    this->headers_parsed = parsed;
}

void    Request::markBodyParsed( const bool & parsed )
{
    this->body_is_parsed = parsed;
}

void    Request::markAsBad( int i )
{
    std::cout << i << std::endl;
    this->is_bad = true;
}

void    Request::markAsReady( const bool & ready )
{
    this->is_ready = ready;
}

void    Request::set_parsingErrorCode( short code )
{
    this->parsingErrorCode = code;
}

void    Request::storeUnparsedMsg(const std::string & msg )
{
    this->unparsed_msg += msg;
}

/* Methods */

void    Request::resetUnparsedMsg()
{
    this->unparsed_msg = "";
}

void    Request::setHeader( const std::string& name, const std::string& value )
{
    if (name == "HOST")
    {
        if (this->headers.find("HOST") != this->headers.end())
            return this->markAsBad(8);
    }
    
    if (this->headers.find(name) != this->headers.end())
        this->headers[name] = value;
    else
        this->headers.insert(std::make_pair(name, value));
}

void    Request::set_body( const std::string & body )
{
    this->body = body;
}

void    Request::print_headrs()
{
    std::cout << "Headers : " << std::endl;
    std::map<std::string, std::string>::iterator it = this->headers.begin();
    std::map<std::string, std::string>::iterator end = this->headers.end();
    for ( ; it != end; it++)
        std::cout << (*it).first << ": " << (*it).second << std::endl;
}
