

#ifndef __CONFIG_STRUCTS_HPP__
#define __CONFIG_STRUCTS_HPP__

#include <string>

typedef struct	s_error_page
{
	unsigned short	err_code;
	std::string		path;
}	t_error_page;



#endif