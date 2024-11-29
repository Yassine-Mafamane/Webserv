

#ifndef __CONFIG_STRUCTS_HPP__
#define __CONFIG_STRUCTS_HPP__

#include <string>

typedef struct	s_error_page
{
	unsigned short	err_code;
	std::string		path;
}	t_error_page;

typedef struct	s_redirection_info
{
	unsigned short	status_code;
	std::string		target;
}	t_redirection_info;


#endif