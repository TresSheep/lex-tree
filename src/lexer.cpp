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
    else if (str[position] == '\n' || str[position] == '\r')
    {
      token_t token;
      token.type = NEWLINE;
      token.content = '\n';
      m_token_stream.push_back(token);
      position += 0;
    }
    else if (str[position] == ':')
    {
      token_t token;
      token.type = SEPARATOR;
      token.content = ':';
      m_token_stream.push_back(token);
      position += 0;
    }
    else if (str[position] == '(')
    {
      token_t token;
      token.type = OPEN_P;
      token.content = '(';
      m_token_stream.push_back(token);
      position += 0;
    }
    else if (str[position] == ')')
    {
      token_t token;
      token.type = CLOSE_P;
      token.content = ')';
      m_token_stream.push_back(token);
      position += 0;
    }
    else if (str[position] == '{')
    {
      token_t token;
      token.type = OPEN_BLOCK;
      token.content = '{';
      m_token_stream.push_back(token);
      position += 0;
    }
    else if (str[position] == '}')
    {
      token_t token;
      token.type = CLOSE_BLOCK;
      token.content = '}';
      m_token_stream.push_back(token);
      position += 0;
    }
    else if (str[position] == '$')
    {
      token_t token;
      token.type = VAR;
      token.content = '$';
      m_token_stream.push_back(token);
      position += 0;
    }
    else if (str[position] == '-' && str[position+1] == '>')
    {
      token_t token;
      token.type = ARROW;
      token.content = "->";
      m_token_stream.push_back(token);
      position += 1;
    }
    else if (str[position] == '<' && str[position+1] == '-' && str[position+2] == '-' && str[position+3] == '-' && str[position+4] == '>')
    {
      token_t token;
      token.type = SECTION_SEPARATOR;
      token.content = "<--->";
      m_token_stream.push_back(token);
      position += 4;
    }
    else if (str[position] == '\"')
    {
      position++;
      token_t token;
      token.type = TOKEN;
      while (str[position] != '\"')
      {
	token.content += str[position];
	position++;

	if (position > str.length())
	  break;
      }
      m_token_stream.push_back(token);
    }
    else if (str[position] == 'c' && str[position+1] == 'r' && str[position+2] == 'e' && str[position+3] == 'a' && str[position+4] == 't' && str[position+5] == 'e')
    {
      token_t token;
      token.type = CREATE;
      token.content = "create";
      m_token_stream.push_back(token);
      position += 5;
    }
    else if (str[position] == 'b' && str[position+1] == 'o' && str[position+2] == 'o' && str[position+3] == 'l' && str[position+4] == 'e' && str[position+5] == 'a' && str[position+6] == 'n')
    {
      token_t token;
      token.type = BOOLEAN;
      token.content = "boolean";
      m_token_stream.push_back(token);
      position += 6;
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
      {
	position++;

	if (position > str.length())
	{
	  m_token_stream.push_back(token);
	  return;
	}
	position--;
      }

      std::string s = "";
      while (IS_NUMBER(str[position]) || str[position] == 'A' || str[position] == 'B' || str[position] == 'C' || str[position] == 'D' || str[position] == 'E' || str[position] == 'F')
      {
	s += str[position];
	position++;
      }
      position--;

      token.content = s;
      
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
