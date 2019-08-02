#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

enum token_type
{
proto_separator,
dot,
separator,
end_statement,
arrow,
open_p,
close_p,
open_block,
close_block,

};

typedef struct token
{
token_type type;
std::string content;
} token_t;

class lexer
{public:
lexer(std::string filename);
~lexer();

void lex(std::string str);
std::vector<token_t> get_token_stream();
private:
std::vector<token_t> m_token_stream;
};