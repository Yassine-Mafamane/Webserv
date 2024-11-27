/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:07:46 by ymafaman          #+#    #+#             */
/*   Updated: 2024/11/27 16:02:33 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

#include "server_setup/Server.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <arpa/inet.h>
#include <map>
#include <netdb.h>
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <queue>
#include <sstream>
#include <fstream>
#include <fcntl.h>


///////////////////////

// Config directives :

// Http context Specefic
#define	SERVER_DIR			"server"
#define	MAX_BODY_DIR		"client_max_body_size"

#define	CGI_EXCT_DIR		"cgi_extention"
#define	AUTO_INDX_DIR		"autoindex"
#define	ERR_PAGE_DIR		"error_page"

// Server context Specefic
#define	LOCATION_DIR		"location"
#define	LISTEN_DIR			"listen"
#define	SERVER_NAMES_DIR	"server_names"
#define	HOST_DIR			"host"
#define	UPLOAD_DIR			"upload_directory"

#define	ALLOWED_METHODS_DIR	"allowed_methods"
#define	INDEX_DIR			"index"
#define	ROOT_DIR			"root"

// Location context Specefic
#define REDIRECTION_DIR		"return"

/////////////////////



typedef struct token_info
{
    std::string     token;
    unsigned int    line_num;
    bool            is_sep; // This indecates if the token is a separator or not. Separators are (";" and "{" and "}")
} token_info;

        /* ----------------------------------------------------------------------------------------------------------------- */

/*  Config parsing functions : @ /config_file_parsing/ */

    // Parse :

void                                    parse_config_file(std::string file_name, HttpContext& http_config);
void                                    store_config(HttpContext& http_config, std::queue<token_info>& tokens_queue, std::string file_name, std::string context);

    // Value extraction :

bool                                    extract_autoindex_value(std::queue<token_info>& tokens_queue, std::string file_name);
std::pair <unsigned short, std::string> extract_error_page_info(std::queue<token_info>& tokens_queue, std::string file_name);
std::string                             extract_cgi_extension(std::queue<token_info>& tokens_queue, std::string file_name);
std::vector<unsigned short>             extract_port_number(std::queue<token_info>& tokens_queue, std::string file_name);
std::string                             extract_root_dir(std::queue<token_info>& tokens_queue, std::string file_name);
std::string                             extract_upload_dir(std::queue<token_info>& tokens_queue, std::string file_name);
std::string                             extract_index(std::queue<token_info>& tokens_queue, std::string file_name);
std::vector<std::string>                extract_srv_names(std::queue<token_info>& tokens_queue, std::string file_name);
size_t                                  extract_max_body_size(std::queue<token_info>& tokens_queue, std::string file_name);
std::vector<std::string>                extract_allowed_methods(std::queue<token_info>& tokens_queue, std::string file_name);
std::pair <unsigned short, std::string> extract_redirection_info(std::queue<token_info>& tokens_queue, std::string file_name);
std::string                             extract_host_name(std::queue<token_info>& tokens_queue, std::string file_name);

    // Token checking :

bool                                    is_http_ctx_dir(std::string directive);
bool                                    is_server_ctx_dir(std::string directive);
bool                                    is_location_ctx_dir(std::string directive);

    // Error throwing :

void                                    throw_config_parse_exception(std::string type, std::string token, std::string file_name, unsigned int line_num);
void                                    throw_wrong_value_exception(std::string directive, std::string wrong_value, std::string file_name, unsigned int line_num);

        /* ----------------------------------------------------------------------------------------------------------------- */


#endif