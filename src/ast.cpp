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
  bool in_main_block = false;
  std::vector<std::string> tokens_to_come_after;
  bool is_child = false;
  std::vector<std::string> names;
  std::vector<block_t*> blocks;
  std::vector<statement*> cases;
  std::vector<_if*> ifs;
  std::vector<std::pair<std::string, statement*>> global_statements;
  std::vector<statement*> case_exprs;
  
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
	    function f(label_name, cases);
	    f.generate_code(m_builder);
	    m_exprs.clear();
	    cases.clear();
	    for (auto block : blocks)
	      delete block;
	    blocks.clear();
	  }

	  label_name = m_token_stream[position].content;

	  position += 1;
	}
	else if (m_token_stream[position+1].type == OPEN_P || m_token_stream[position+1].type == IDENTIFIER)
	{
	  // Case

	  names.clear();
	  while (m_token_stream[position].type == IDENTIFIER)
	  {
	    names.push_back(m_token_stream[position].content);
	    position++;
	  }

	  // Check if parent or declaration of what can come after
	  
	  is_child = false;
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
	  tokens_to_come_after.clear();
	  while (m_token_stream[position].type != CLOSE_P)
	  {
	    tokens_to_come_after.push_back(m_token_stream[position].content);
	    position++;
	  }

	  if (m_token_stream[position+1].type != OPEN_BLOCK)
	  {
	    if (m_token_stream[position+1].type != NEWLINE)
	    {
	      std::cout << "ERROR: expected \"{\" after \")\" in case! Line: " << line << "\n";
	      exit(0);
	    }
	    else
	      line++;
	  }

	  position++;

	  block_t* blck = new block_t();
	  blck->nested_pos = 1;
	  blck->taken_care_of = false;
	  blocks.push_back(blck);
	}
	else if (m_token_stream[position+1].type == ARROW)
	{
	  if (m_token_stream[position+2].type != IDENTIFIER)
	  {
	    std::cout << "ERROR: expected identifier after \"->\"! Line: " << line << "\n";
	    exit(0);
	  }

	  // Parse variable
	  var* v = new var(name, m_token_stream[position+2].content);
	  global_statements.push_back(std::pair<std::string, statement*>(name, v));

	  position += 2;
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
      case CLOSE_BLOCK:
      {
	// Close the back block

	for (int i = blocks.size() - 1; i != -1; i--)
	{
	  if (!blocks[i]->taken_care_of)
	  {
	    if (blocks[i]->nested_pos == 1)
	    {
	      // Case
	      _case* c = new _case(names, case_exprs);
	      cases.push_back(c);
	      case_exprs.clear();
	    }
	    else
	    {
	      case_exprs.push_back(blocks[i]->stmnt);
	    }

	    blocks[i]->taken_care_of = true;
	  }
	}
      } break;
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

	create* _create = new create(s1, args_to_process);
	blocks.back()->statements.push_back(_create);
      } break;
      case IF:
      {
	std::string lhs = m_token_stream[position+1].content;
	if (m_token_stream[position+1].type != IDENTIFIER)
	{
	  std::cout << "ERROR: expected identifier after \"if\"! Line: " << line << "\n";
	  exit(0);
	}

	// Check if it is a global statement
	bool found = false;
	for (auto stmnt : global_statements)
	{
	  if (stmnt.first.compare(lhs) == 0)
	    found = true;
	}

	if (!found)
	{
	  std::cout << "ERROR: unexpected identifierfier: " << lhs << "! Line: " << line << "\n";
	  exit(0);
	}

	if (m_token_stream[position+2].type != IS) // or is_not
	{
	  std::cout << "ERROR: expected \"is\" in \"if\"! Line: " << line << "\n";
	  exit(0);
	}
	std::string type = m_token_stream[position+2].content;

	if (m_token_stream[position+3].type != TRUE) // or false
	{
	  std::cout << "ERROR: expected \"true\" or \"false\" in \"if\"! Line: " << line << "\n";
	  exit(0);
	}
	std::string rhs = m_token_stream[position+3].content;

	if (m_token_stream[position+4].type != OPEN_BLOCK)
	{
	  if (m_token_stream[position+4].type != NEWLINE)
	  {
	    std::cout << "ERROR: expected \"true\" or \"false\" in \"if\"! Line: " << line << "\n";
	    exit(0);
	  }
	  else
	    line++;
	}

	position += 4;

	block_t* blck = new block_t();
	_if* i = new _if(lhs, type, rhs, blck->statements);
	blck->stmnt = i;
	blck->nested_pos = blocks.back()->nested_pos + 1;
	blck->taken_care_of = false;

	blocks.push_back(blck);
	// Parse logic
      } break;
      default:
	std::cout << "ERROR: unexpeted token: " << m_token_stream[position].content << "! Line: " << line << "\n";
	exit(0);
	break;
      }
    }
  }
  
  function f(label_name, cases);
  f.generate_code(m_builder);
  m_exprs.clear();
  cases.clear();
  for (auto block : blocks)
    delete block;
  blocks.clear();
}
