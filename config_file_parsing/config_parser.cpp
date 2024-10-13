/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:04:07 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/13 21:53:31 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parse.hpp"
#include <unistd.h>

/* Removing the escape characters from the token before storing it */

token_info normalize_token(std::string token, unsigned int line_num)
{
    bool        removed_one;
    bool        escaped;
    char        quote;

    if (token == ";" || token == "{" || token == "}")
        return {token, line_num, true};

    removed_one = false;
    escaped = false;
    
    /* if i remove quotes first then do the following step i will not know if the character im trying to escape was withing quotes or not. */
    for (size_t i = 0; i < token.length(); i++)
    {
        if (removed_one)
            i--;
         
        if ((token[i] == '\\') && !removed_one && (i != token.length() - 1) && ((token[i + 1] == '\\')
            || (token[i + 1] == ';') || (token[i + 1] == '{') || (token[i + 1] == '}')
            || (token[i + 1] == '#') || (token[i + 1] == '\'') || (token[i + 1] == '"')))
        {
            token.erase(i, 1);
            escaped = false;
            removed_one = true;
        }
        else if ((token[i] == '\'' || token[i] == '"') && !escaped)
        {
            quote = token[i];
            token.erase(i, 1);
            removed_one = true;
            
            while (i < token.length())
            {
                if (token[i] == quote)
                {
                    token.erase(i, 1);
                    break ;
                }
                i++;
            }
        }
        else
        {
            escaped = false;   
            removed_one = false;
        }
    }

    return {token, line_num, false};
}

std::string get_next_chunk(std::string token)
{
    unsigned int    escape_char_counter;
    char            quote;

    escape_char_counter = 0;

    for (size_t i = 0; i < token.length(); i++)
    {
        if ((token[i] == '"' || token[i] == '\'') && !(escape_char_counter % 2))
        {
            quote = token[i];
            i++;
            while (i < token.length())
            {
                if ((token[i] == quote) && !(escape_char_counter % 2))
                    break ;
                i++;
            }
            i++;
        }

        if (token[i] == '\\')
            escape_char_counter++;

        if ((token[i] == ';' || token[i] == '{' || token[i] == '}') && !(escape_char_counter % 2))
        {
            if (!i)
                return token.substr(0, 1);
            else
                return token.substr(0, i);
        }

        if (token[i] != '\\')
            escape_char_counter = 0;
    }
    return token;
}

/* this function's job is to only store tokens and skip comments */

bool    has_unmatched_quote(std::string token)
{
    unsigned int    escape_char_counter;
    bool            unmatched_quote_found;
    char            quote;

    escape_char_counter = 0;

    for (size_t i = 0; i < token.length(); i++)
    {
        if (token[i] == '\\')
        {
            escape_char_counter++;
        }
        else if (!unmatched_quote_found && (token[i] == '\'' || token[i] == '"') && !(escape_char_counter % 2))
        {
            unmatched_quote_found = true;
            quote = token[i];
        }
        else if (unmatched_quote_found && (token[i] == quote) && !(escape_char_counter % 2))
        {
            unmatched_quote_found = false;
        }

        if (token[i] != '\\')
            escape_char_counter = 0;
    }
    return unmatched_quote_found;
}

void    append_token_to_queue(std::string token, std::queue<token_info>& tokens_queue, std::string file_name, unsigned int line_num)
{
    std::string chunk;
    size_t      chunk_pos;

    (void) file_name;
    (void) line_num;
    while (token.length())
    {
        if (token[0] == '#')
            break;

        chunk = get_next_chunk(token);

        tokens_queue.push(normalize_token(chunk, line_num));

        chunk_pos = token.find(chunk);
        token.erase(chunk_pos, chunk.length());
    }
}


void    tokenize_config(std::queue<token_info>& tokens_queue, std::string file_name)
{
    unsigned    int         line_num;
    std::stringstream       strm;
    std::ifstream           file;
    std::string             token;
    std::string             line;
    std::string             word;
    
    file.open(file_name);
    if (!file.is_open())
        throw std::runtime_error("Failed to open the configuration file!");
    
    line_num = 1;
    
    while (std::getline(file, line))
    {
        strm.clear();
        strm.str(line);

        strm >> word;

        while (word != "")
        {
            token += word;

            if (token[0] == '#')
                break;

            if (!has_unmatched_quote(token))
            {
                append_token_to_queue(token, tokens_queue, file_name, line_num);
                token = "";
            }

            word = "";
            strm >> word;
        }

        line_num++;
    }

    /* throwing an exception in case EOF was reached but a quote is unmatched. */
    if (!token.empty())
        throw_config_parse_exception("EOF", "", file_name, line_num); 
}

void    config_tokenizer(std::string file_name)
{
    std::queue<token_info>  tokens_queue;
    HttpContext             http_config;


    tokenize_config(tokens_queue, file_name);

    if (tokens_queue.front().token != "http")
    {
        if (tokens_queue.front().token == ";" || tokens_queue.front().token == "{" || tokens_queue.front().token == "}")
            throw_config_parse_exception("Unexpected", tokens_queue.front().token, file_name, tokens_queue.front().line_num);
        else if (is_http_ctx_dir(tokens_queue.front().token) || is_server_ctx_dir(tokens_queue.front().token) || is_location_ctx_dir(tokens_queue.front().token))
            throw_config_parse_exception("Not allowed", tokens_queue.front().token, file_name, tokens_queue.front().line_num);
        else
            throw_config_parse_exception("Unknown", tokens_queue.front().token, file_name, tokens_queue.front().line_num);
    }
    tokens_queue.pop();

    while (!tokens_queue.empty())
    {
        std::cout << tokens_queue.front().token << std::endl;
        tokens_queue.pop();
    }

    // store_config(http_config, tokens_queue, file_name, "http");

    // http_config.show_info();
}