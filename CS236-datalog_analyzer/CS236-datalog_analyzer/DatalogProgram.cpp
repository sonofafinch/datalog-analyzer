/* ---------------------------------------------------------------------------
**
** DatalogProgram.cpp
** Datalog structure class definition.
** 7-5-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "DatalogProgram.h"

void DatalogProgram::error()
{
	throw cur_token;
}

void DatalogProgram::advance(vector<Token>& tokens)
{
	if (cur_token < tokens.size())
	{
		++cur_token;
	}
	else
	{
		matchTerminal(Token::ENDFILE, tokens);
	}
}

void DatalogProgram::matchTerminal(Token::TOKEN_NUM token_type, vector<Token> &tokens)
{
	if (tokens[cur_token].getType() == Token::TOKEN_TYPE[token_type])
	{
		advance(tokens);
	}
	else
	{
		error();
	}
}

void DatalogProgram::parameter(vector<Token>& tokens)
{
	string param = tokens[cur_token].getValue();
	if (tokens[cur_token].getType() == "STRING")
	{
		cur_param.push_back(Parameter(true, tokens[cur_token].getType(), tokens[cur_token].getValue()));
		domain.emplace(tokens[cur_token].getValue());
		matchTerminal(Token::STRING, tokens);
	}
	else if (tokens[cur_token].getType() == "ID")
	{
		cur_param.push_back(Parameter(false, tokens[cur_token].getType(), tokens[cur_token].getValue()));
		matchTerminal(Token::ID, tokens);
	}
	else
	{
		error();
	}
}

void DatalogProgram::parameterList(vector<Token>& tokens)
{
	if (tokens[cur_token].getType() == "COMMA")
	{
		matchTerminal(Token::COMMA, tokens);
		parameter(tokens);
		parameterList(tokens);
	}
	else
	{
		;
	}
}

void DatalogProgram::predicate(vector<Token>& tokens)
{
	cur_param.clear();
	parameter(tokens);
	string cur_name = cur_param.front().toString();
	cur_param.clear();
	matchTerminal(Token::LEFT_PAREN, tokens);
	parameter(tokens);
	parameterList(tokens);
	matchTerminal(Token::RIGHT_PAREN, tokens);
	cur_pred.push_back(Predicate(cur_name, cur_param));
}

void DatalogProgram::predicateList(vector<Token>& tokens)
{
	if (tokens[cur_token].getType() == "COMMA")
	{
		matchTerminal(Token::COMMA, tokens);
		predicate(tokens);
		predicateList(tokens);
	}
	else
	{
		;
	}
}

void DatalogProgram::scheme(vector<Token>& tokens)
{
	cur_pred.clear();
	predicate(tokens);
	schemes.push_back(cur_pred[0]);
}

void DatalogProgram::schemeList(vector<Token>& tokens)
{
	if (tokens[cur_token].getType() == "ID")
	{
		scheme(tokens);
		schemeList(tokens);
	}
	else
	{
		;
	}
}

void DatalogProgram::fact(vector<Token>& tokens)
{
	cur_pred.clear();
	predicate(tokens);
	facts.push_back(cur_pred[0]);
	matchTerminal(Token::PERIOD, tokens);
}

void DatalogProgram::factList(vector<Token>& tokens)
{
	if (tokens[cur_token].getType() == "ID")
	{
		fact(tokens);
		factList(tokens);
	}
	else
	{
		;
	}
}

void DatalogProgram::rule(vector<Token>& tokens)
{
	cur_pred.clear();
	predicate(tokens);
	Predicate cur_name = cur_pred[0];
	cur_pred.clear();
	matchTerminal(Token::COLON_DASH, tokens);
	predicate(tokens);
	predicateList(tokens);
	matchTerminal(Token::PERIOD, tokens);
	rules.push_back(Rule(cur_name, cur_pred));
}

void DatalogProgram::ruleList(vector<Token>& tokens)
{
	if (tokens[cur_token].getType() == "ID")
	{
		rule(tokens);
		ruleList(tokens);
	}
	else
	{
		;
	}
}

void DatalogProgram::query(vector<Token>& tokens)
{
	cur_pred.clear();
	predicate(tokens);
	queries.push_back(cur_pred[0]);
	matchTerminal(Token::Q_MARK, tokens);
}

void DatalogProgram::queryList(vector<Token>& tokens)
{
	if (tokens[cur_token].getType() == "ID")
	{
		query(tokens);
		queryList(tokens);
	}
	else
	{
		;
	}
}

void DatalogProgram::parse(vector<Token> tokens)
{
	matchTerminal(Token::SCHEMES, tokens);
	matchTerminal(Token::COLON, tokens);
	scheme(tokens);
	schemeList(tokens);
	matchTerminal(Token::FACTS, tokens);
	matchTerminal(Token::COLON, tokens);
	factList(tokens);
	matchTerminal(Token::RULES, tokens);
	matchTerminal(Token::COLON, tokens);
	ruleList(tokens);
	matchTerminal(Token::QUERIES, tokens);
	matchTerminal(Token::COLON, tokens);
	query(tokens);
	queryList(tokens);
	matchTerminal(Token::ENDFILE, tokens);
}

string DatalogProgram::toString()
{
	stringstream ss;
	ss << "Success!\n";
	ss << "Schemes(" << schemes.size() << "):\n";
	for (vector<Predicate>::iterator it = schemes.begin(); it != schemes.end(); ++it)
	{
		ss << "  " << it->toString() << "\n";
	}
	ss << "Facts(" << facts.size() << "):\n";
	for (vector<Predicate>::iterator it = facts.begin(); it != facts.end(); ++it)
	{
		ss << "  " << it->toString() << "\n";
	}
	ss << "Rules(" << rules.size() << "):\n";
	for (vector<Rule>::iterator it = rules.begin(); it != rules.end(); ++it)
	{
		ss << "  " << it->toString() << "\n";
	}
	ss << "Queries(" << queries.size() << "):\n";
	for (vector<Predicate>::iterator it = queries.begin(); it != queries.end(); ++it)
	{
		ss << "  " << it->toString() << "\n";
	}
	ss << "Domain(" << domain.size() << "):\n";
	for (set<string>::iterator it = domain.begin(); it != domain.end(); ++it)
	{
		ss << "  " << *it << "\n";
	}
	return ss.str();
}