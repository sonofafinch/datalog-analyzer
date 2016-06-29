/* ---------------------------------------------------------------------------
**
** lexer.h
** Lexer/scanner class declaration
** 6-28-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once
#include <fstream>
#include <vector>
#include "token.h"

class Lexer
{
public:
	Lexer()
	{
		err_bell = false;
		input = 0;
		line_num = 1;
		token_count = 0;
	}
	void buildToken(string token_name, string token_value, int cur_line);
	void keyword_compare(string check_string);
	void default_case(ifstream& cur_file);
	void string_case(ifstream& cur_file);
	void comment_case(ifstream& cur_file);
	void colon_case(ifstream& cur_file);
	void scan(char* argv);
	string toString();

private:
	bool err_bell;
	char input;
	int line_num;
	int token_count;
	vector<Token> token_list;
	vector<string> cur_string;
};
