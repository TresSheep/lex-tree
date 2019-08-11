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
  std::string label_name = "";
  std::vector<std::string> after_case;
  
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

	  if (!m_exprs.empty())
	  {
	    function f(label_name, std::move(m_exprs));
	    f.generate_code(m_builder);
	    m_exprs.clear();
	  }

	  label_name = m_token_stream[position].content;

	  position++;
	}
	else if (m_token_stream[position+1].type == OPEN_P || m_token_stream[position+1].type == IDENTIFIER)
	{
	  // Case

	  std::vector<std::string> names;
	  while (m_token_stream[position].type == IDENTIFIER)
	  {
	    names.push_back(m_token_stream[position].content);
	    position++;
	  }

	  // Check if parent or declaration of what can come after
	  
	  bool is_child = false;
	  for (auto name : names)
	  {
	    for (auto token : after_case)
	    {
	      if (token.compare(name) == 0)
	      {
	        is_child = true;
	        break;
	      }
	    }
	  }

	  position++;
	  std::vector<std::string> tokens_to_come_after;
	  while (m_token_stream[position].type != CLOSE_P)
	  {
	    tokens_to_come_after.push_back(m_token_stream[position].content);
	    position++;
	  }

	  // Add case
	  m_exprs.push_back(std::move(std::make_unique<_case>(names, tokens_to_come_after, is_child)));
	  if (!is_child)
	  {
	    after_case = tokens_to_come_after;
	  }
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
      case CREATE:
      {
	std::vector<size_t> args_to_process;
	std::string s1 = m_token_stream[position+1].content;
	if (m_token_stream[position + 1].type != IDENTIFIER)
	{
	  std::cout << "ERROR: expected an identifier after \"create\"! Line: " << line << "\n";
	  exit(0);
	}

	if (m_token_stream[position + 2].type != OPEN_P)
	{
	  std::cout << "ERROR: expected '(' in \"create\"! Line: " << line << "\n";
	  exit(0);
	}
	
	position += 3;
	while (m_token_stream[position].type != CLOSE_P)
	{
	  if (m_token_stream[position].type == VAR)
	  {
	    // Get the arg index
	    if (m_token_stream[position + 1].type != DEC_NUMBER)
	    {
	      std::cout << "ERROR: expected number after '$' in \"create\"! Line: " << line << "\n";
	      exit(0);
	    }

	    position += 2;

	    // Create arg in "create"
	    args_to_process.push_back(std::stoi(m_token_stream[position - 1].content));
	  }
	  else if (m_token_stream[position].type == IDENTIFIER)
	  {
	    position++;
	  }
	  else
	  {
	    std::cout << "ERROR: expected '$' or identifier in \"create\"! Line: " << line << "\n";
	    exit(0);
	  }
	}
	
	m_exprs.push_back(std::move(std::make_unique<create>(s1, args_to_process)));
      } break;
      default:
	std::cout << "ERROR: unexpeted token: " << m_token_stream[position].content << "! Line: " << line << "\n";
	exit(0);
	break;
      }
    }
  }

  function f(label_name, std::move(m_exprs));
  f.generate_code(m_builder);
  m_exprs.clear();
}
