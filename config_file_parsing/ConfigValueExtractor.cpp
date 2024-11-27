

#include "ConfigValueExtractor.hpp"

ConfigValueExtractor::ConfigValueExtractor(std::queue<token_info> & tokens) : tokens_queue(tokens)
{

}

std::string	ConfigValueExtractor::extract_single_string_value()
{
	std::string	value;
	token_info	directive;
	token_info	token;

	directive = tokens_queue.front();

    tokens_queue.pop();
    token = tokens_queue.front();

	if (token.is_sep)
		ConfigException::throwParsingError(UNEXPECTED, token);
	else
		value = token.token;

	tokens_queue.pop();
	token = tokens_queue.front();

	validate_directive_ending(token, directive);

	tokens_queue.pop();
	return value;
}

std::vector<std::string>	ConfigValueExtractor::extract_multi_string_value()
{
	std::vector<std::string>	value;
	token_info					directive;
	token_info					token;

	directive = tokens_queue.front();

    tokens_queue.pop();
    token = tokens_queue.front();

	if (token.is_sep)
		ConfigException::throwParsingError(UNEXPECTED, token);

	while (!tokens_queue.empty() && !token.is_sep)
	{
		value.push_back(token.token);
		tokens_queue.pop();
		token = tokens_queue.front();
	}

	validate_directive_ending(token, directive);

	tokens_queue.pop();
	return value;
}

std::vector<unsigned int>	ConfigValueExtractor::extract_port_nums()
{
	std::vector<unsigned int>	value;
	token_info					directive;
	token_info					token;

	directive = tokens_queue.front();

    tokens_queue.pop();
    token = tokens_queue.front();

	if (token.is_sep)
		ConfigException::throwParsingError(UNEXPECTED, token);

	while (!tokens_queue.empty() && !token.is_sep)
	{
		validate_port_number(token);
		value.push_back(std::stoi(token.token));
		tokens_queue.pop();
		token = tokens_queue.front();
	}

	validate_directive_ending(token, directive);

	tokens_queue.pop();
	return value;
}

size_t    ConfigValueExtractor::extract_max_body_size()
{
	token_info	token;
	token_info	directive;
	size_t		size = 0;

	directive = tokens_queue.front();

    tokens_queue.pop();
    token = tokens_queue.front();

    if (token.is_sep)
		ConfigException::throwParsingError(UNEXPECTED, token);
    
	validate_max_body_size(token);

	size = std::stoul(token.token);

    tokens_queue.pop();
    token = tokens_queue.front();

    validate_directive_ending(token, directive);

	tokens_queue.pop();

    return size;
}

void	ConfigValueExtractor::validate_directive_ending(const token_info & token, const token_info & directive)
{
	if (token.is_sep && token.token == ";")
		return ;

	if (tokens_queue.empty() || token.is_sep)
		ConfigException::throwParsingError(UNTERMINATED, directive);
	else
		ConfigException::throwParsingError(WRONG_ARGS_NUM, directive);
}


void	ConfigValueExtractor::validate_port_number(const token_info & token)
{
	if (!is_all_digits(token.token)
		|| (token.token.length() != 4 && token.token.length() != 5)
		|| (token.token.length() == 4 && token.token < "1024" )
		|| (token.token.length() == 5 && token.token > "49151"))
	{	
		ConfigException::throwWrongValueError(LISTEN_DIR, token);
	}
}

void	ConfigValueExtractor::validate_max_body_size(const token_info & token)
{
	if (!is_all_digits(token.token)
		|| (token.token.length() > 11)
		|| ((token.token.length() == 11) && (token.token > "12500000000")))
	{
		ConfigException::throwWrongValueError(MAX_BODY_DIR, token);
	}
}