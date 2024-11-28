
#ifndef __CONFIG_VALUE_EXTRACTER__
#define __CONFIG_VALUE_EXTRACTER__

#include "ConfigException.hpp"

class ConfigValueExtractor {

	public :

		/* Constructors */
		ConfigValueExtractor(std::queue<token_info> & tokens_queue);

		std::string					extract_single_string_value(void (ConfigValueExtractor::*validator)(const token_info &));
		std::vector<std::string>	extract_multi_string_value();
		std::vector<unsigned int>	extract_port_nums();
		size_t						extract_max_body_size();
		
		void						validate_directive_ending(const token_info & token, const token_info & directive);
		void						validate_port_number(const token_info & token);
		void						validate_max_body_size(const token_info & token);
		void						validate_auto_indx_value(const token_info & token);

	private :

		std::queue<token_info> & tokens_queue;

};







#endif