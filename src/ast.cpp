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
  std::string data_def_name = "";
  std::string data_def_value = "";
  std::string label_name = "";
  
  for (auto position = 0; position < m_token_stream.size(); position++)
  {
    switch (m_token_stream[position].type)
    {
    }
  }
}
