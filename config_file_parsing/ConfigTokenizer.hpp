


#ifndef __CONFIG_TOKENIZER__
#define __CONFIG_TOKENIZER__

#include <string>
#include "../webserv.hpp"

class ConfigTokenizer {

	public :

		static std::queue<token_info>   tokenize(const std::string & file_name);

    private :

		ConfigTokenizer();

		~ConfigTokenizer();

        void    	open_config_file(std::string file_name);
		std::queue<token_info>		start_tokenization();
		void		process_line_tokens();
		void		extract_quoted_token();
		void		append_token_to_queue();
		std::string	get_quoted_string();
		std::string	capture_chars_after_quote();
		std::string	get_next_chunk();
		token_info	normalize_token(std::string & token);

		/* --------------------- Utils --------------------- */

		void	remove_leading_spaces();
		char	leading_quote_or_wspace(const std::string & line);

        /* ------------------------------------------------------------------------------ */

		std::queue<token_info>	tokens_queue;
        std::ifstream			file;
		std::string				file_name;

		std::string				line;			// Stores the current line from the confing file being parsed
		std::string				token;
        unsigned int			line_num;

		std::stringstream		streamed_line;

		char					quote;
		bool					pair_quote_found;
};


#endif