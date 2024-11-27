

#ifndef __CONFIG_PARSER_HPP__
# define __CONFIG_PARSER_HPP__

#include "../webserv.hpp"

class ConfigParser
{
	public :

		static HttpContext	get_config(const std::string & file_name); // this function will wrapp all the parsing process



};


#endif