#include "lexer.h"

// macro a-z
#define IS_LOW_ALPHA(character) character >= 'a' && character <= 'z' || character == '_'

// macro A-Z
#define IS_HIGH_ALPHA(character) character >= 'A' && character <= 'Z' || character == '_'

#define IS_NUMBER(character) character >= '0' && character <= '9'


lexer::lexer(std::string filename)
{
  std::ifstream stream(filename);
  if (!stream.is_open())
  {
    std::cout << "Could not read the file: " << filename << "!\n";
    exit(0);
  }

  std::stringstream ss;
  ss << stream.rdbuf();
  ss.seekp(0, std::ios::end);
  auto size = ss.tellp();
  ss.seekp(0, std::ios::beg);
  m_token_stream.reserve(size);
  
  for (std::string str; std::getline(ss, str);)
  {
    lex(str);
  }

  stream.close();
}

lexer::~lexer()
{
}

void lexer::lex(std::string str)
{
  for (size_t position = 0; position < str.length(); position++)
  {
    if (str[position] == '/' && str[position+1] == '/')
    {
      token_t token;
      token.type = NEWLINE;
      token.content = '\n';
      m_token_stream.push_back(token);
      position = str.length(); // Comment
    }
    else if (str[position] == ' ')
      position += 0; // Do nothing
    else if (str[position] == '\n')
    {
      token_t token;
      token.type = NEWLINE;
      token.content = '\n';
      m_token_stream.push_back(token);
      position += 0;
    }
    else if (IS_HIGH_ALPHA(str[position]) || IS_LOW_ALPHA(str[position])) {
      token_t token;
      token.type = IDENTIFIER;
      while (IS_HIGH_ALPHA(str[position]) || IS_LOW_ALPHA(str[position]) || IS_NUMBER(str[position])) {
	token.content += str[position];
	position++;
      }
      position--;
      m_token_stream.push_back(token);
    }
    else if (IS_NUMBER(str[position])) {
      bool hex = false;
      token_t token;
      token.type = DEC_NUMBER;
      if (str[position] == '0' && str[position + 1] == 'x')
	token.type = HEX_NUMBER;

      position += 2;
      while (IS_NUMBER(str[position]) || str[position] == 'A' || str[position] == 'B' || str[position] == 'C' || str[position] == 'D' || str[position] == 'E' || str[position] == 'F') {
	token.content += str[position];
	position++;
      }
      
      position--;
      m_token_stream.push_back(token);
    }
    else
    {
      std::cout << "ERROR: Unexpected token: " << str[position] << "!\n";
      exit(0);
    }
  }
}

std::vector<token_t> lexer::get_token_stream()
{
  return m_token_stream;
}
