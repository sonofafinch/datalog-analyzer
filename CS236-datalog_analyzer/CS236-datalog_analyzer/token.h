/* ---------------------------------------------------------------------------
**
** token.h
** Token class declaration/definition
** 7-2-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once
#include <string>
#include <sstream>

using namespace std;

class Token
{
public:
	enum TOKEN_NUM
	{
		COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH,
		SCHEMES, FACTS, RULES, QUERIES, ID, STRING, ENDFILE
	};
	static const string TOKEN_TYPE[14];
	static const string TOKEN_NAME[14];
	Token()
	{
		line = 0;
	}
	Token(string token_name, string token_value, int cur_line)
	{
		type = token_name;
		value = token_value;
		line = cur_line;
	}
	string getType()
	{
		return type;
	}
	string getValue()
	{
		return value;
	}
	string toString()
	{
		stringstream output;
		output << "(" << type << "," << "\"" << value << "\"," << line << ")";
		return output.str();
	}

private:
	string type;
	string value;
	int line;
};
