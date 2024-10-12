/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationContext.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 11:31:25 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/12 20:57:10 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_CONTEXT_HPP
# define LOCATION_CONTEXT_HPP

#include <vector>
#include <string>
#include <iostream>


class LocationContext {

    public :

    /* LocationContext Constructor */
    LocationContext( void );

    /* LocationContext Destructor */
    ~LocationContext();

    /* Setters */

    void    set_error_page      ( const std::pair<unsigned short, std::string>& error_info );
    void    set_redirection     ( std::pair<unsigned short, std::string> redirection_info );
    void    set_root_directory  ( std::string root );
    void    set_cgi_extension   ( const std::string& extension );
    void    set_upload_dir      ( std::string directory );
    void    set_index           ( std::string index );
    void    set_auto_index      ( bool on_off );
    void    set_allowed_methods ( std::vector<std::string> methods );
    void    make_exact          ( void );


    private :

        std::vector<std::pair <unsigned short, std::string> >   error_pages;

        std::pair <unsigned short, std::string>                 redirection; // TODO : Check if multiple redirections are allowed.

        std::vector<std::string>                                allowed_methods;  

        std::string                                             location;

        std::string                                             root_directory;

        std::string                                             cgi_extension;

        std::string                                             upload_dir;

        std::string                                             index;

        bool                                                    auto_index;

        bool                                                    is_exact;
};


#endif