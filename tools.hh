/**
 * Helper functions
 *
 * @author Tuomas Angervuori <tuomas.angervuori@gmail.com>
 */

#ifndef TOOLS_HH
#define TOOLS_HH

#include <string>
#include <vector>
#include <iostream>

using namespace std;

namespace tools {
	
	/**
	 * Split text into vector components using separator
	 *
	 * @param separator Separator string used for splitting the text
	 * @param text Text to be split
	 * @param limit Maximum number of vector parts, 0=no limits
	 * @return a vector containing the splitted test
	 */
	vector<string> split(const string &separator, const string &text, const unsigned int &limit);
	vector<string> split(const string &separator, const string &text);
}

#endif
