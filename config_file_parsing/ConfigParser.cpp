

#include "ConfigParser.hpp"


ConfigParser::ConfigParser(std::queue<token_info> & tokens) : tokens_queue(tokens), extractor(tokens)
{
	is_inside_serv = false;
}


HttpContext	ConfigParser::getConfig(const std::string & file_name)
{
    // Tokenizing the config file :
    std::queue<token_info>  tokens = ConfigTokenizer::tokenize(file_name);

    // Setting the file_name attribute for the ConfigException class.
	ConfigException::file_name = file_name;

	validate_tokens_queue(tokens);

	ConfigParser	parser(tokens);

	// Removing the "http" token from the top of the queue to start processing directives.
    parser.tokens_queue.pop();

	parser.storeConfig("http");

}

void    ConfigParser::validate_tokens_queue(const std::queue<token_info> & tokens_queue)
{
    if (tokens_queue.empty())
        ConfigException::throwParsingError(EMPTY, tokens_queue.front());

	// Making sure the first token value is "http"
	if (tokens_queue.front().token != "http")
    {
		if (tokens_queue.front().is_sep)
			ConfigException::throwParsingError(UNEXPECTED, tokens_queue.front());
		else if (is_a_valid_dir(tokens_queue.front().token))
			ConfigException::throwParsingError(NOT_ALLOWED, tokens_queue.front());
		else
			ConfigException::throwParsingError(UNKNOWN, tokens_queue.front());
	}
}

void	ConfigParser::storeConfig(const std::string & context)
{
	bool	is_inside_lct = false;

	if (tokens_queue.front().token != "{")
	    ConfigException::throwParsingError(NO_OPENING, tokens_queue.front());

	tokens_queue.pop();

	while (!tokens_queue.empty() && tokens_queue.front().token != "}")
	{
	    /* Here i will be extracting directives and store them one by one */
	    if (context == "http")
	    {
	        is_inside_serv = false;
	        storeHttpDirs();
	    }
	    else if (context == "server")
	    {
	        if (!is_inside_serv)
	            http_config.set_new_server();
	        is_inside_serv = true;
	        store_serv_directives(http_config, tokens_queue, file_name);
	    }
	    else if (context == "location")
	    {
	        store_location_directives(http_config.get_latest_server().get_latest_location(), tokens_queue, file_name);
	    }
	}

	if (tokens_queue.empty())
	    throw_config_parse_exception("unclosed_ctx", "", file_name, tokens_queue.front().line_num);

	tokens_queue.pop();

	if (context == "http" && !tokens_queue.empty())
	    throw_config_parse_exception("Unexpected", tokens_queue.front().token, file_name, tokens_queue.front().line_num);
}

void	ConfigParser::find_bad_token_type(token_info & token)
{
	if (token.is_sep)
		ConfigException::throwParsingError(UNEXPECTED, token);
	else if (is_a_valid_dir(token.token))
		ConfigException::throwParsingError(NOT_ALLOWED, tokens_queue.front());
	else
		ConfigException::throwParsingError(UNKNOWN, tokens_queue.front());
}

void	ConfigParser::storeHttpDirs()
{
	token_info	token = tokens_queue.front();

	if (!is_http_ctx_dir(token.token))
		find_bad_token_type(token);
	
	if (token.token == SERVER_DIR)
	{
		tokens_queue.pop();
		storeConfig("server");
	}
	else if (token.token == AUTO_INDX_DIR)
	{
		std::string	value = extractor.extract_single_string_value(&ConfigValueExtractor::validate_auto_indx_value);
		http_config.set_auto_index(value);
	}
}

