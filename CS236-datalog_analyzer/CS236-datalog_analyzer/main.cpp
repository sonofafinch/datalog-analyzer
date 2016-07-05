/* ---------------------------------------------------------------------------
**
** main.cpp
** Runs a scanner with input from command arguments and outputs similarly
** 7-5-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "lexer.h"
#include "DatalogProgram.h"

int main(int argc, char* argv[])
{
	Lexer luthor;
	luthor.scan(argv[1]);
	DatalogProgram brainiac;
	brainiac.parse(luthor.getTokens());
	ofstream outfile;
	outfile.open(argv[2]);
	outfile << brainiac.toString();
	outfile.close();
	return 0;
}
