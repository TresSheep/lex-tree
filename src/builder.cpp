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
    m_ast_functions << "std::unique_ptr<" << name << "> ast<builder>::parse_" << name <<
      "(size_t& position)\n{\n" <<
      "switch (m_token_stream[position].type)\n{\ndefault:\nstd::cout << \"Unexpected token\";\nexit(0);\nbreak;\n";

  }

  m_ast_inl_stream << "parse_" << name << "(position);\n";
}

void builder::add_function_end()
{
  for (auto arg : m_case_args)
  {
  }
  
  m_ast_functions << "\n}\n}";
}

void builder::add_case(std::vector<std::string> args, std::vector<std::string> tokens_to_come_after, bool child)
{
  case_arg_t* arg;
  arg->call_stack = args;
  arg->after = tokens_to_come_after;

  /*
  if (child)
  {
    if (m_case_args.size() > 0)
      arg->parent = m_case_args.back();
    else
      arg->parent = 0;
  }
  else
    arg->parent = 0;
  */

  //m_case_args.push_back(arg);

  /*
  if (!child)
  {
    m_ast_functions << "case " << args[0] << ":\n{\n";

    size_t i = 0;
    for (auto token : args)
    {
      if (i != 0)
      {
        m_ast_functions << "if (!m_token_stream[position+" << std::to_string(i) << "].type != " << token << ")\n{\n"
			<< "std::cout << \"ERROR: Expected token: \" << m_token_stream[position+" << std::to_string(i) << "].content << \" after token: \" << m_token_stream[position+" << std::to_string(i-1) << "].content;\nexit(0)\n}\n"
		      << "std::string s" << std::to_string(i) << " = m_token_stream[position+" << std::to_string(i) << "].content;\n";
      }
 
      i++;
    }

    m_ast_functions << "position += " << std::to_string(i) << ";\n";

    i = 0;
    for (auto token : tokens_to_come_after)
    {
      m_ast_functions << "if (!m_token_stream[position+" << std::to_string(i) << "].type != " << token << ")\n{\n"
		    << "std::cout << \"ERROR: Expected token: " << token << " after expression\";\nexit(0)\n}\n";

      i++;
    }


    m_ast_functions << "\n} break;\n";
    }
  else
  {
  }*/
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
