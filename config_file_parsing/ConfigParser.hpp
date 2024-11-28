

#ifndef __CONFIG_PARSER_HPP__
# define __CONFIG_PARSER_HPP__

#include "ConfigTokenizer.hpp"
#include "ConfigException.hpp"
#include "ConfigValueExtractor.hpp"

class ConfigParser
{
	public :

		static HttpContext	getConfig(const std::string & file_name); // this function will wrapp all the parsing process
		static void			validate_tokens_queue(const std::queue<token_info> & tokens_queue);
		void				storeConfig(const std::string & context);
		void				storeHttpDirs();
		void				storeServDirs();
		void				find_bad_token_type(token_info & token);


	private :

		// Constructor :
		ConfigParser(std::queue<token_info> & tokens);

		std::queue<token_info>	&	tokens_queue;
		HttpContext					http_config; // This represents the output of the parsing process
		ConfigValueExtractor		extractor;

		bool						is_inside_serv;
};


#endif