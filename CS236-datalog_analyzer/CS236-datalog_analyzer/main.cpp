/* ---------------------------------------------------------------------------
**
** main.cpp
** Runs a scanner with input from command arguments and outputs similarly
** 6-28-16
** Author: Nathan Finch
** -------------------------------------------------------------------------*/

#include "lexer.h"

int main(int argc, char* argv[])
{
	Lexer luthor;
	luthor.scan(argv[1]);
	ofstream outfile;
	outfile.open(argv[2]);
	outfile << luthor.toString();
	outfile.close();
	return 0;
}
