/* ---------------------------------------------------------------------------
**
** lexer.cpp
** Lexer class definition
** 6-28-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "lexer.h"

const string Token::TOKEN_TYPE[14] = { "COMMA","PERIOD","Q_MARK","LEFT_PAREN","RIGHT_PAREN","COLON","COLON_DASH",
			"SCHEMES","FACTS","RULES","QUERIES","ID","STRING","EOF" };
const string Token::TOKEN_NAME[14] = { ",",".","?","(",")",":",":-",
		"Schemes","Facts","Rules","Queries","ID","STRING","" };


void Lexer::buildToken(string TOKEN_TYPE, string token_value, int cur_line)
{
	Token cur_token(TOKEN_TYPE, token_value, cur_line);
	token_list.push_back(cur_token);
}

void Lexer::keyword_compare(string check_string)
{
	if (check_string == "Schemes")
	{
		buildToken(Token::TOKEN_TYPE[Token::SCHEMES],Token::TOKEN_NAME[Token::SCHEMES],line_num);
	}
	else if(check_string == "Facts")
	{
		buildToken(Token::TOKEN_TYPE[Token::FACTS],Token::TOKEN_NAME[Token::FACTS],line_num);
	}
	else if(check_string == "Rules")
	{
		buildToken(Token::TOKEN_TYPE[Token::RULES],Token::TOKEN_NAME[Token::RULES],line_num);
	}
	else if(check_string == "Queries")
	{
		buildToken(Token::TOKEN_TYPE[Token::QUERIES],Token::TOKEN_NAME[Token::QUERIES],line_num);
	}
	else
	{
		buildToken(Token::TOKEN_TYPE[Token::ID],check_string,line_num);
	}
}

void Lexer::default_case(ifstream& cur_file)
{
	if (isalpha(input))
	{
		stringstream ss;
		ss << input;
		cur_string.push_back(ss.str());
		while (isalnum(cur_file.peek()))
		{
			stringstream ss2;
			input = cur_file.get();
			ss2 << input;
			cur_string.push_back(ss2.str());
		}
	}
	else if (isspace(input))
	{
		return;
	}
	else
	{
		err_bell = true;
		return;
	}
	string token_check;
	for (vector<string>::iterator it = cur_string.begin(); it != cur_string.end(); ++it)
	{
		token_check += *it;
	}
	keyword_compare(token_check);
	cur_string.clear();
}

void Lexer::string_case(ifstream& cur_file)
{
	stringstream ss;
	ss << input;
	cur_string.push_back(ss.str());
	while (cur_file.peek() != '\'')
	{
		stringstream ss2;
		input = cur_file.get();
		if((input == '\n') | (input == EOF))
		{
			err_bell = true;
			return;
		}
		ss2 << input;
		cur_string.push_back(ss2.str());
	}
	stringstream ss3;
	input = cur_file.get();
	ss3 << input;
	cur_string.push_back(ss3.str());
	string token_check;
	for (vector<string>::iterator it = cur_string.begin(); it != cur_string.end(); ++it)
	{
		token_check += *it;
	}
	buildToken(Token::TOKEN_TYPE[Token::STRING], token_check, line_num);
	cur_string.clear();
}

void Lexer::comment_case(ifstream& cur_file)
{
	while (cur_file.peek() != '\n' && !cur_file.eof())
	{
		input = cur_file.get();
	}
}

void Lexer::colon_case(ifstream& cur_file)
{
	if (cur_file.peek() == '-')
	{
		input = cur_file.get();
		buildToken(Token::TOKEN_TYPE[Token::COLON_DASH], Token::TOKEN_NAME[Token::COLON_DASH], line_num);
		return;
	}
	else
	{
		buildToken(Token::TOKEN_TYPE[Token::COLON], Token::TOKEN_NAME[Token::COLON], line_num);
		return;
	}
}

void Lexer::scan(char* argv)
{
	ifstream infile(argv);
	if (!infile.is_open())
	{
		return;
	}
	while (infile.peek() != EOF && err_bell == false)
	{
		input = infile.get();
		switch (input)
		{
			case ',':
				buildToken(Token::TOKEN_TYPE[Token::COMMA], Token::TOKEN_NAME[Token::COMMA], line_num);
				break;
			case '.':
				buildToken(Token::TOKEN_TYPE[Token::PERIOD], Token::TOKEN_NAME[Token::PERIOD], line_num);
				break;
			case '?':
				buildToken(Token::TOKEN_TYPE[Token::Q_MARK], Token::TOKEN_NAME[Token::Q_MARK], line_num);
				break;
			case '(':
				buildToken(Token::TOKEN_TYPE[Token::LEFT_PAREN], Token::TOKEN_NAME[Token::LEFT_PAREN], line_num);
				break;
			case ')':
				buildToken(Token::TOKEN_TYPE[Token::RIGHT_PAREN], Token::TOKEN_NAME[Token::RIGHT_PAREN], line_num);
				break;
			case '\'':
				string_case(infile);
				break;
			case ':':
				colon_case(infile);
				break;
			case '#':
				comment_case(infile);
				break;
			case '\n':
				++line_num;
				break;
			default:
				default_case(infile);
				break;
		}
	}
	input = infile.get();
	if((input == EOF) && err_bell != true)
	{
		buildToken(Token::TOKEN_TYPE[Token::ENDFILE], Token::TOKEN_NAME[Token::ENDFILE], line_num);
	}
	infile.close();
}

string Lexer::toString()
{
	stringstream ss;
	for (vector<Token>::iterator it = token_list.begin(); it != token_list.end(); ++it)
	{
		ss << it->toString() << "\n";
		++token_count;
	}
	if(err_bell != true)
	{
		ss << "Total Tokens = " << token_count << "\n";
	}
	else
	{
		ss << "Input Error on line " << line_num << "\n";
	}
	return ss.str();
}
