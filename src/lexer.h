#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

enum token_type
{
 NEWLINE,
 IDENTIFIER,
 DEC_NUMBER,
 HEX_NUMBER,
 TOKEN,
 SECTION_SEPARATOR,
 SEPARATOR,
 OPEN_P,
 CLOSE_P,
 OPEN_BLOCK,
 CLOSE_BLOCK,
 CREATE,
 VAR,
 ARROW,
 CONTINUE,
 IF,
 ELIF,
 ELSE,
 IS,
 TRUE
};

typedef struct token
{
  token_type type;
  std::string content;
} token_t;

class lexer
{
 public:
  lexer(std::string filename);
  ~lexer();

  void lex(std::string str);

  std::vector<token_t> get_token_stream();

 private:
  std::vector<token_t> m_token_stream;
};
