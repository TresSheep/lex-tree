#include "builder.h"

builder::builder()
{
}

builder::~builder()
{
}

void builder::add_token(std::string name, std::string content)
{
  bool special = false;
  m_lexer_cpp_stream << "if (";
  std::string s = "";
  std::string after_str = "";
  size_t after_pos = 0;

  if (content.find("*") != std::string::npos)
  {
    if (special)
    {
      after_pos = content.find("*") + 1;
    }
  }

  if (content.find("[A-Z]") != std::string::npos)
  {
    special = true;

    if (content.find("[A-Z]") < after_pos)
    {
      after_str += "IS_HIGH_ALPHA(str[position])||";
    }
    else
    {
      s += "IS_HIGH_ALPHA(str[position])||";
    }
  }
  if (content.find("[a-z]") != std::string::npos)
  {
    special = true;

    if (content.find("[a-z]") < after_pos)
    {
      after_str += "IS_LOW_ALPHA(str[position])||";
    }
    else
    {
      s += "IS_LOW_ALPHA(str[position])||";
    }
  }
  if (content.find("[0-9]") != std::string::npos)
  {
    special = true;

    if (content.find("[0-9]") > after_pos)
    {
      after_str += "IS_NUMBER(str[position])||";
    }
    else
    {
      s += "IS_NUMBER(str[position])||";
    }
  }

  if (special)
  {
    if (!after_str.empty())
    {
      after_str.pop_back();
      after_str.pop_back();
    }
    else
    {
      s.pop_back();
      s.pop_back();
    }

    m_lexer_cpp_stream << s.c_str();
    m_lexer_cpp_stream <<
      ")\n{\ntoken_t token;\ntoken.type = " << name << ";\n" <<
      "while (" << s << after_str << ")\n{\ntoken.content += str[position];\n" <<
      "position++;\n}\nposition--;\nm_token_stream.push_back(token);\n}\n";

    return;
  }
  
  size_t i = 0;
  for (char c : content)
  {
    s += "str[position+";
    s += std::to_string(i);
    s += "] == '";
    s += c;
    s += "'&&";
    i++;
  }
  
  s.pop_back();
  s.pop_back();

  m_lexer_cpp_stream << s.c_str();

  m_lexer_cpp_stream <<
    ")\n{\ntoken_t token;\ntoken.type = " << name << ";\n" <<
    "token.content = \"" << content << "\";\n" <<
    "m_token_stream.push_back(token);\n" <<
    "position += " << std::to_string(content.length() - 1) << ";\n}\n";
  m_lexer_h_stream << name << ",\n";
}

void builder::add_function_start(std::string name)
{
  if (name.compare("main") == 0)
  {
    m_ast_functions << "template <typename builder>\n";
    m_ast_functions << "void ast<builder>::parse_" << name <<
      "(size_t& position)\n{\n" <<
      "switch (m_token_stream[position].type)\n{\ndefault:\nstd::cout << \"Unexpected token\";\nexit(0);\nbreak;\n";

  }

  m_ast_inl_stream << "parse_" << name << "(position);\n";

  m_current_function = name;
}

void builder::add_function_end()
{
  std::vector<size_t> checked;
  std::vector<same_arg_t*> same_arg;
  for (size_t i = 0; i < m_case_args.size(); i++)
  {
    for (size_t j = 0; j < m_case_args.size(); j++)
    {
      if (m_case_args[j] != m_case_args[i])
      {
	if (m_case_args[j]->call_stack[0].compare(m_case_args[i]->call_stack[0]) == 0)
	{
	  bool is_checked = false;
	  for (size_t c : checked)
	  {
	    if (i == c || j == c)
	      is_checked = true;
	  }

	  if (!is_checked)
	  {
	    // Has the first arg togheter
	    same_arg_t* s = new same_arg_t();
	    s->arg1 = m_case_args[i];
	    s->arg2 = m_case_args[j];
	    s->index = 1;
	    same_arg.push_back(s);
	    
	    // check is 2/3/4 ... is the same

	    size_t x = 0;
	    if (x <= m_case_args[j]->call_stack.size() || x <= m_case_args[i]->call_stack.size())
	    {
	      while (m_case_args[j]->call_stack[x].compare(m_case_args[i]->call_stack[x]) == 0)
	      {
		x++;
		if (x > m_case_args[j]->call_stack.size() - 1 || x > m_case_args[i]->call_stack.size() - 1)
		{
		  break;
		}

		if (m_case_args[j]->call_stack[x].compare(m_case_args[i]->call_stack[x]) == 0)
		{
		  // The same
		  same_arg_t* s = new same_arg_t();
		  s->arg1 = m_case_args[i];
		  s->arg2 = m_case_args[j];
		  s->index = x + 1;
		  same_arg.push_back(s);
		}
	      }
	    }
	    
	    checked.push_back(i);
	    checked.push_back(j);
	  }
	}
      }
    }
  }
  
  // Process case
  bool parent = false;
  bool has_same = false;
  for (auto _case : m_case_args)
  {
    for (auto arg : same_arg)
    {
      if (arg->arg1 == _case)
      {
	if (arg->index == 1)
	{
	  // Parent case
	  m_ast_functions << "case " << arg->arg1->call_stack[arg->index - 1] << ":\n{\n";
	  parent = true;
	  has_same = true;
	}
	else
	{
	  // Child case but the same
	  m_ast_functions << "if (m_token_stream[position+" << std::to_string(arg->index - 1) << "].type == " << arg->arg1->call_stack[arg->index - 1] << ")\n{\n";

	  // Parse arg1 and arg2
	  size_t x = 0;
	  bool has_extra = false;
	  for (auto call : arg->arg1->call_stack)
	  {
	    if (x == arg->index - 1)
	    {
	      m_ast_functions << "if (m_token_stream[position+" << std::to_string(x + 1) << "].type == " << arg->arg1->call_stack[x + 1] << ")\n{\n";
	      has_extra = true;
	    }
	    x++;
	  }

	  m_ast_functions << arg->arg1->create;

	  if (x > 1)
	  {
	    if (has_extra)
	      m_ast_functions << "break;\n";
	    m_ast_functions << "}\n";
	  }

	  x = 0;
	  for (auto call : arg->arg2->call_stack)
	  {
	    if (x == arg->index - 1)
	    {
	      if (x + 1 < arg->arg2->call_stack.size())
		m_ast_functions << "if (m_token_stream[position+" << std::to_string(x + 1) << "].type == " << arg->arg2->call_stack[x + 1] << ")\n{\n";
	    }
	    x++;
	  }

	  m_ast_functions << arg->arg2->create;
	  
	  m_ast_functions << "}\n";
	}
      }
      else if (arg->arg2 == _case)
      {
	has_same = true;
      }
    }
    
    if (parent)
      m_ast_functions << "\n} break;\n";

    if (!has_same)
    {
      m_ast_functions << "case " << _case->call_stack[0] << ":\n{\n";

      m_ast_functions << "std::string s1 = m_token_stream[position].content;\n";

      size_t x = 0;
      for (auto call : _case->call_stack)
      {
	if (x != 0)
	{
	  m_ast_functions << "if (m_token_stream[position+" << std::to_string(x) << "].type != " << call << ")\n{\n"
			  << "std::cout << \"ERROR: Unexpected token! Line: \" << m_line << \"\\n\";\n"
			  << "exit(0);\n}\n"
			  << "std::string s" << std::to_string(x + 1) << " = m_token_stream[position+" << std::to_string(x) << "].content;\n";
	}

	x++;
      }

      m_ast_functions << _case->create;
      m_ast_functions << "\n} break;\n";
    }
    else
    {
    }
    
    parent = false;
    has_same = false;
  }
  
  m_ast_functions << "\n}\n}";

  m_case_args.clear();
  same_arg.clear();
  m_functions.push_back(m_current_function);
  m_current_function.clear();
}

void builder::add_case(std::vector<std::string> args)
{
  case_arg_t* arg = new case_arg_t();
  arg->call_stack = args;
  m_case_args.push_back(arg);
}

void builder::add_create(std::string obj, std::vector<size_t> args)
{
  if (m_current_function.compare("main") == 0)
  {
    m_create = obj;
    m_create += " obj(";
    bool f = false;
    for (auto arg : args)
    {
      for (auto func : m_functions)
      {
	for (auto _case : m_case_args)
	{
	  if (func.compare(_case->call_stack[arg - 1]) == 0)
	  {
	    // It is a function
	    f = true;
	  }
	}
      }

      if (!f)
      {
	// It is not a function
	m_create += "s";
	m_create += std::to_string(arg);
	m_create += ",";
      }

      f = false;
    }
    if (m_create.back() == ',')
      m_create.pop_back(); // ,
    m_create += ");\nobj.generate_code(m_builder);\n";
  }

  m_case_args.back()->create = m_create;
  m_create.clear();
}

void builder::save()
{
  std::ofstream lexer_h_stream("lexer.h");

  // lexer.h
  lexer_h_stream << "#pragma once\n\n#include <string>\n#include <fstream>\n";
  lexer_h_stream << "#include <sstream>\n#include <iostream>\n#include <vector>\n\n";
  lexer_h_stream << "enum token_type\n{\n";
  
  lexer_h_stream << m_lexer_h_stream.str();

  lexer_h_stream << "\n};\n\ntypedef struct token\n{\ntoken_type type;\n";
  lexer_h_stream << "std::string content;\n} token_t;\n\nclass lexer\n{public:\n";
  lexer_h_stream << "lexer(std::string filename);\n~lexer();\n\n";
  lexer_h_stream << "void lex(std::string str);\nstd::vector<token_t> get_token_stream();\n";
  lexer_h_stream << "private:\nstd::vector<token_t> m_token_stream;\n};";
  
  // lexer.cpp

  std::ofstream lexer_cpp_stream("lexer.cpp");

  lexer_cpp_stream << "#include \"lexer.h\"\n";
  
  lexer_cpp_stream <<
    "#define IS_LOW_ALPHA(c) c >= 'a' && c <= '<' || c == '_'\n" <<
    "#define IS_HIGH_ALPHA(c) c >= 'A' && c <= 'Z' || c == '_'\n" <<
    "#define IS_NUMBER(c) c >= '0' && c <= '9'\n" <<
    "lexer::lexer(std::string filename)\n{\n" <<
    "std::ifstream stream(filename);\nif (!stream.is_open())\n{\n" <<
    "std::cout << \"Could not read the file: \" << filename << \"!\\n\";\n" <<
    "exit(0);\n}\nstd::stringstream ss;\nss << stream.rdbuf();\n" <<
    "ss.seekp(0, std::ios::end);\nauto size = ss.tellp();\n" <<
    "ss.seekp(0, std::ios::beg);\nm_token_stream.reserve(size);\n" <<
    "for (std::string str; std::getline(ss, str);)\n{\nlex(str);\n}\nstream.close();\n}\n" <<
    "lexer::~lexer()\n{\n}\nvoid lexer::lex(std::string str)\n{\n" <<
    "for (size_t position = 0; position < str.length(); position++)\n{\n";

  lexer_cpp_stream << m_lexer_cpp_stream.str();

  lexer_cpp_stream << "\n}\n}\nstd::vector<token_t> lexer::get_token_stream()\n" <<
    "{\nreturn m_token_stream;\n}\n";

  // Save ast

  std::ofstream ast_h_stream("ast.h");

  ast_h_stream << "#pragma once\n#include \"lexer.h\"\n" <<
    "#include <memory>\ntemplate <typename builder>\n" <<
    "class ast\n{\npublic:\nast(std::vector<token_t> token_stream, builder& b);\n~ast();\n" <<
    "void parse();\nprivate:\nstd::vector<token_t> m_token_stream;\nbuilder& m_builder;\n};";

  std::ofstream ast_inl_stream("ast.inl");

  ast_inl_stream << "#include \"ast.h\"\ntemplate <typename builder>\n" <<
    "ast<builder>::ast(std::vector<token_t> token_stream, builder& b) :\n" <<
    "m_token_stream(token_stream), m_line(1), m_builder(b)\n{\n}\n" <<
    "template <typename builder>\nast<builder>::~ast()\n{\n}\n" <<
    "template <typename builder>\nvoid ast<builder>::parse()\n{\n" <<
    "for (size_t position = 0; position < m_token_stream.size(); position++)\n{\n";
  
  ast_inl_stream << m_ast_inl_stream.str();

  ast_inl_stream << "\n}\n}\n";

  ast_inl_stream << m_ast_functions.str();
  
  lexer_h_stream.close();
  lexer_cpp_stream.close();
  ast_h_stream.close();
  ast_inl_stream.close();

  m_case_args.clear();
}
