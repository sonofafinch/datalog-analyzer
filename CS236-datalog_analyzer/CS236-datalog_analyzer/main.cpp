/* ---------------------------------------------------------------------------
**
** main.cpp
** Runs a scanner with input from command arguments and outputs similarly
** 7-5-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "lexer.h"
#include "DatalogProgram.h"
#include <sstream>

using namespace std;

int main(int argc, char* argv[])
{
	Lexer luthor;
	luthor.scan(argv[1]);
	DatalogProgram brainiac;
	ofstream outfile;
	outfile.open(argv[2]);
	try
	{
		brainiac.parse(luthor.getTokens());
	}
	catch (int e)
	{
		stringstream ss;
		ss << "Failure!\n" << "  " << luthor.getTokens()[e].toString() << "\n";
		outfile << ss.str();
		return 0;
	}
	outfile << brainiac.toString();
	outfile.close();
	return 0;
}
