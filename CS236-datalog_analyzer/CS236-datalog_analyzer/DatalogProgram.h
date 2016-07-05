/* ---------------------------------------------------------------------------
**
** DatalogProgram.h
** Datalog structure class declaration.
** 7-5-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#pragma once
#include "lexer.h"
#include "Predicate.h"
#include "Rule.h"
#include <vector>
#include <set>

using namespace std;

class DatalogProgram
{
public:
	DatalogProgram()
	{
		cur_token = 0;
	}

	void error();
	void advance(int cur_token, vector<Token>& tokens);
	void matchTerminal(Token::TOKEN_NUM token_type, vector<Token>& tokens);
	void parameter(vector<Token>& tokens);
	void parameterList(vector<Token>& tokens);
	void predicate(vector<Token>& tokens);
	void predicateList(vector<Token>& tokens);
	void scheme(vector<Token>& tokens);
	void schemeList(vector<Token>& tokens);
	void fact(vector<Token>& tokens);
	void factList(vector<Token>& tokens);
	void rule(vector<Token>& tokens);
	void ruleList(vector<Token>& tokens);
	void query(vector<Token>& tokens);
	void queryList(vector<Token>& tokens);
	void parse(vector<Token> tokens);
	string toString();

private:
	int cur_token;
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Predicate> queries;
	vector<Rule> rules;
	vector<Parameter> domain;
	vector<Parameter> cur_param;
	vector<Predicate> cur_pred;
	set<string> domain;
};