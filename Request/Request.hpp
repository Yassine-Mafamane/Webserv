/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 17:07:06 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/08 17:48:22 by ymafaman         ###   ########.fr       */
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
        std::string get_query();
        std::string get_method();
        std::string get_body();
        bool    hasParsedStartLine();
        bool    hasParsedHeaders();
        bool    hasParsedBody();
        bool    isBadRequest();
        bool    isReady();
        std::string getUnparsedMsg();
        std::string get_version();

        /* Setters */
        void    set_method( const std::string & method );
        void    set_version( const std::string & version );
        void    set_target( const std::string & target );
        void    set_query( std::string query );
        void    set_body( const std::string & body );
        void    markStartLineParsed( const bool & parsed ); // TODO: remove argument ?
        void    markHeadersParsed( const bool & parsed );
        void    markBodyParsed( const bool & parsed );
        void    markAsReady( const bool & ready );
        void    markAsBad( int i );
        void    set_parsingErrorCode( short code );
        void    storeUnparsedMsg(const std::string & msg );

        /* Methods */
        void    resetUnparsedMsg();
        void    setHeader( const std::string& name, const std::string& value );

        void    print_headrs();

    private : 

        std::map<std::string, std::string>  headers;
        std::string                         method;
        std::string                         version;
        std::string                         target;
        std::string                         body;
        std::string                         query;

        bool                                start_line_is_parsed;
        bool                                headers_parsed;
        bool                                body_is_parsed;
        bool                                is_ready;

        /*  */
        bool                                is_bad;
        short                               parsingErrorCode;

        /* */
        std::string                         unparsed_msg;
};








#endif