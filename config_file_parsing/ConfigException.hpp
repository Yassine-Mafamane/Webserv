
#ifndef __CONFIG_EXCEPTION__
# define __CONFIG_EXCEPTION__

#include "../webserv.hpp"

// TODO : can be wrapped inside a namespace!
typedef enum	e_ConfigParseError
{
	EMPTY,
	UNEXPECTED,
	NOT_ALLOWED,
	UNKNOWN,
	UNCLOSED_CTX,
	DUPLICATION,
	UNTERMINATED,
	WRONG_ARGS_NUM,
	END_OF_FILE,
	NO_OPENNING
}	ConfigParseError;

class ConfigException {

	public :

		static void	throwParsingError(const ConfigParseError & e, const token_info & token);
		static void	ConfigException::throwWrongValueError(const std::string & directive, const token_info & token);
		/*	*/

		static std::string  file_name;

};




#endif