/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerContext.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:31:30 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/12 20:58:40 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_CONTEXT_HPP
# define SERVER_CONTEXT_HPP

#include <vector>
#include <string>
#include <iostream>
#include "LocationContext.hpp"

class ServerContext {

    public :
        
        /* ServerContext Constructor */
        ServerContext( void );

        /* ServerContext Destructor */
        ~ServerContext();

        /* Setters */
        void    set_error_page      ( const std::pair <unsigned short, std::string>& error_info );
        void    set_new_location    ( void );
        void    set_port            ( unsigned short port );
        void    set_root_directory  ( std::string root );
        void    set_cgi_extension   ( const std::string& extension );
        void    set_upload_dir      ( std::string directory );
        void    set_index           ( std::string index );
        void    set_server_names    ( std::vector<std::string> names );
        void    set_auto_index      ( bool on_off );
        void    set_allowed_methods ( std::vector<std::string> methods );

        /* The following method returns the latest inserted locationContext in the locations vector in order to store it's info. */
        LocationContext&  get_latest_location( void );

    private :

        std::vector<std::pair <unsigned short, std::string> >   error_pages;

        std::vector<LocationContext>                            locations; // TODO : Is making a separated vector for exact locations better that mixing all locations in one single vector. 

        std::vector<std::string>                                server_names;

        std::vector<std::string>                                allowed_methods;

        unsigned    short                                       port;

        std::string                                             root_directory;

        std::string                                             cgi_extension;

        std::string                                             upload_dir;

        std::string                                             index;

        bool                                                    auto_index;       
};



#endif


