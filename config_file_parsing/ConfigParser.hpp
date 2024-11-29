

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

		void				validate_context_closure(const std::string & context);

		void				storeHttpDirs();
		void				storeServDirs();
		void				storelocationDirs();

		void				find_bad_token_type(token_info & token);
		bool				is_duplicated_http_dir(const std::string & directive);
		bool				is_duplicated_serv_dir(const std::string & directive, const ServerContext & serv);
		bool				is_duplicate_location_dir(const std::string & directive, const LocationContext & location);
		void				setup_new_location();
		void				setup_new_server();

	private :

		// Constructor :
		ConfigParser(std::queue<token_info> & tokens);

		std::queue<token_info>	&	tokens_queue;
		HttpContext					http_config; // This represents the output of the parsing process
		ConfigValueExtractor		extractor;
};


#endif