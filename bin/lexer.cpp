#include "lexer.h"
#define IS_LOW_ALPHA(c) c >= 'a' && c <= '<' || c == '_'
#define IS_HIGH_ALPHA(c) c >= 'A' && c <= 'Z' || c == '_'
#define IS_NUMBER(c) c >= '0' && c <= '9'
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
if (str[position+0] == ':'&&str[position+1] == ':')
{
token_t token;
token.type = proto_separator;
token.content = "::";
m_token_stream.push_back(token);
position += 1;
}
if (IS_HIGH_ALPHA(str[position])||IS_LOW_ALPHA(str[position])||)
{
token_t token;
token.type = identifier;
while (IS_HIGH_ALPHA(str[position])||IS_LOW_ALPHA(str[position])||IS_NUMBER(str[position]))
{
token.content += str[position];
position++;
}
position--;
m_token_stream.push_back(token);
}
if (IS_NUMBER(str[position]))
{
token_t token;
token.type = number;
while (IS_NUMBER(str[position]))
{
token.content += str[position];
position++;
}
position--;
m_token_stream.push_back(token);
}
if (str[position+0] == '.')
{
token_t token;
token.type = dot;
token.content = ".";
m_token_stream.push_back(token);
position += 0;
}
if (str[position+0] == ',')
{
token_t token;
token.type = separator;
token.content = ",";
m_token_stream.push_back(token);
position += 0;
}
if (str[position+0] == ';')
{
token_t token;
token.type = end_statement;
token.content = ";";
m_token_stream.push_back(token);
position += 0;
}
if (str[position+0] == '-'&&str[position+1] == '>')
{
token_t token;
token.type = arrow;
token.content = "->";
m_token_stream.push_back(token);
position += 1;
}
if (str[position+0] == '(')
{
token_t token;
token.type = open_p;
token.content = "(";
m_token_stream.push_back(token);
position += 0;
}
if (str[position+0] == ')')
{
token_t token;
token.type = close_p;
token.content = ")";
m_token_stream.push_back(token);
position += 0;
}
if (str[position+0] == '{')
{
token_t token;
token.type = open_block;
token.content = "{";
m_token_stream.push_back(token);
position += 0;
}
if (str[position+0] == '}')
{
token_t token;
token.type = close_block;
token.content = "}";
m_token_stream.push_back(token);
position += 0;
}

}
}
std::vector<token_t> lexer::get_token_stream()
{
return m_token_stream;
}
