/**
 * Helper functions
 *
 * @author Tuomas Angervuori <tuomas.angervuori@gmail.com>
 */
#include <string>
#include <vector>
#include <iostream>
#include "tools.hh"

using namespace std;

namespace tools {
	
	vector<string> split(const string &separator, const string &text, const unsigned int &limit) {
		
		vector<string> result;
		
		unsigned int startpos = 0;
		unsigned int endpos;
		
		//No limit for the vector size
		if(limit == 0) {
			while(startpos < text.size()) {
				
				string tempstring = text.substr(startpos,text.size());
				
				//Find the separator string from the text
				endpos = tempstring.find_first_of(separator);
				
				if(endpos>text.size()) {
					endpos = text.size();
				}
				
				result.push_back(tempstring.substr(0,endpos));
				
				startpos = startpos + endpos + separator.size();
			}
		}
		
		//Vector size has a maximum limit
		else {
			unsigned int count = 1;
			while((startpos < text.size()) && (count <= limit)) {
				string tempstring = text.substr(startpos,text.size());
				
				if(count < limit) {
					endpos = tempstring.find_first_of(separator);
					
					if(endpos>text.size()) {
						endpos = text.size();
					}
				}
				else {
					endpos = text.size();
				}
				
				result.push_back(tempstring.substr(0,endpos));
				
				startpos = startpos + endpos + separator.size();
				count++;
			}
		}
		
		return result;
	}
	
	vector<string> split(const string &separator, const string &text) {
		return split(separator, text, 0);
	}
}
