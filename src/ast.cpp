#include "ast.h"

ast::ast(std::vector<token_t> token_stream, builder& b) :
  m_token_stream(token_stream), m_line(1), m_builder(b)
{
}

ast::~ast()
{
}

void ast::parse()
{
  bool lexer_section = true;
  size_t line = 1;
  
  for (auto position = 0; position < m_token_stream.size(); position++)
  {
    if (m_token_stream[position].type == SECTION_SEPARATOR)
    {
      lexer_section = false;
      position++;
    }

    if (lexer_section)
    {
      switch (m_token_stream[position].type)
      {
      case IDENTIFIER:
      {
	if (m_token_stream[position+1].type != TOKEN)
	{
	  std::cout << "ERROR: expected token after identifier! Line: " << line << "\n";
	  exit(0);
	}

	m_builder.add_token(m_token_stream[position].content, m_token_stream[position+1].content);

	position += 1;
      } break;
      case NEWLINE:
      {
	line++;
      } break;
      default:
      {
	std::cout << "ERROR: unexpeted token: " << m_token_stream[position].content << "! Line: " << line << "\n";
	exit(0);
      } break;
      }
    }
    else
    {
      switch (m_token_stream[position].type)
      {
      case IDENTIFIER:
      {
	std::string name = m_token_stream[position].content;
	
	if (m_token_stream[position+1].type == SEPARATOR)
	{
	  // Function

	  position++;
	}
	else if (m_token_stream[position+1].type == OPEN_P)
	{
	  // Case

	  position += 2;
	  std::vector<std::string> tokens_to_come_after;
	  
	  while (m_token_stream[position].type != CLOSE_P)
	  {
	    tokens_to_come_after.push_back(m_token_stream[position].content);
	    position++;
	  }

	  // Add case
	}
	else
	{
	  std::cout << "ERROR: expected \":\" or \"(\" after identifier! Line: " << line << "\n";
	  exit(0);
	}
      } break;
      case NEWLINE:
	line++;
	break;
      case OPEN_BLOCK:
      {
      } break;
      case CLOSE_BLOCK:
      {
      } break;
      default:
	std::cout << "ERROR: unexpeted token: " << m_token_stream[position].content << "! Line: " << line << "\n";
	exit(0);
	break;
      }
    }
  }
}
