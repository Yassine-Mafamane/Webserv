/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:07:06 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/05 21:47:56 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <map>

class Request {

    public :

        /* Constructors */
        Request( void );

        /* Destructor */
        ~Request();

        /* Getters */
        std::string get_target();
        bool    hasParsedStartLine();
        bool    isBadRequest();

        /* Setters */
        void    set_method( const std::string& method );
        void    set_version( const std::string& version );
        void    set_target( const std::string& target );
        void    set_query( std::string query );
        void    markStartLineParsed( const bool& parsed );
        void    markAsBad();
        void    set_parsingErrorCode( short code );
    
    private : 

        std::map<std::string, std::string>  headers; // for now it will be a vector but latter queries might be stored in a map
        std::string                         method;
        std::string                         version;
        std::string                         target;
        std::string                         body;
        std::string                         query;

        bool                                start_line_is_parsed;

        /*  */
        bool                                is_bad;
        short                               parsingErrorCode;
    
};








#endif