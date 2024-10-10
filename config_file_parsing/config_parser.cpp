/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_parser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymafaman <ymafaman@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 15:04:07 by ymafaman          #+#    #+#             */
/*   Updated: 2024/10/10 21:20:40 by ymafaman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config_parse.hpp"
#include <unistd.h>

/* Removing the escape characters from the token before storing it */

std::string normalize_token(std::string token)
{
    bool    removed_one;

    removed_one = false;

    for (size_t i = 0; i < token.length(); i++)
    {
        if (removed_one)
            i--;

        if ((token[i] == '\\') && !removed_one && (i != token.length() - 1) && ((token[i + 1] == '\\') || (token[i + 1] == ';') || (token[i + 1] == '{') || (token[i + 1] == '}')))
        {
            token.erase(i, 1);
            removed_one = true;
        }
        else
            removed_one = false;
    }
    return token;
}

std::string get_next_chunk(std::string token)
{
    unsigned int    escape_char_counter;

    escape_char_counter = 0;

    for (size_t i = 0; i < token.length(); i++)
    {
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

void    append_token_to_queue(std::string token, std::queue<std::string>& tokens_queue, std::string file_name, unsigned int line_num)
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
        
        tokens_queue.push(normalize_token(chunk));

        chunk_pos = token.find(chunk);
        token.erase(chunk_pos, chunk.length());
    }
}

void tokenize_line(std::queue<std::string>& tokens_queue, std::string line, std::string file_name, unsigned int line_num)
{
    std::stringstream       strm(line);
    std::string             token;
    
    strm >> token;

    while (!token.empty())
    {
        if (token[0] == '#')
        {
            break;
        }

        append_token_to_queue(token, tokens_queue, file_name, line_num);

        token = "";
        strm >> token;
    }
    
}

void    config_tokenizer(std::string file_name)
{
    std::queue<std::string> tokens_queue;
    unsigned    int         line_num;
    std::ifstream           file;
    std::string             line;

    file.open(file_name);

    line_num = 1;

    if (!file.is_open())
        throw std::runtime_error("Failed to open the configuration file!");

    while (std::getline(file, line))
    {
        tokenize_line(tokens_queue, line, file_name, line_num);
        line_num++;
    }

    while (!tokens_queue.empty())
    {
        std::cout << tokens_queue.front() << std::endl;
        tokens_queue.pop();
    }
}