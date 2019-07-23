#pragma once
#include "lexer.h"
#include <memory>
template <typename builder>
class ast
{
public:
ast(std::vector<token_t> token_stream, builder& b);
~ast();
void parse();
private:
std::vector<token_t> m_token_stream;
builder& m_builder;
};