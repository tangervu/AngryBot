/**
 * AngryBot IRC bot
 * 
 * A useless irc bot written only for educational purposes
 * 
 * @author Tuomas Angervuori <tuomas.angervuori@gmail.com>
 */
#include <iostream>
#include <vector>
#include <getopt.h>

#include "irc.hh"

using namespace std;

int main(int argc, char **argv) {
	
	cout << "Welcome to AngryBot!" << endl << endl;
	
	//Connection parameters
	string host = "localhost", nick = "angrybot", name = "AngryBot sample bot", channel = "#angrybot";
	int option, option_index = 0;
	static struct option long_options[] = {
		{"host", required_argument, 0, 'h'},
		{"nick", 1, 0, 'u'},
		{"name", 1, 0, 'n'},
		{"channel", 1, 0, 'c'}
	};
	while((option = getopt_long(argc, argv, "h:u:n:c:", long_options, &option_index)) != -1) {
		if(option == 'h') {
			host = optarg;
		}
		else if(option == 'u') {
			nick = optarg;
		}
		else if(option == 'n') {
			name = optarg;
		}
		else if(option == 'c') {
			channel = optarg;
		}
	}
	
	
	vector<irc::response> response;
	
	cout << "Connecting to " << host << ": " << endl;
	irc::Irc irc(host);
	irc.nick(nick);
	irc.login(nick, name);
	irc.join(channel);
	
	//Initial response
	while(true) {
		response = irc.waitForData();
		for(vector<irc::response>::size_type i=0; i < response.size(); i++) {
			//Received a private message from another user
			if(response[i].command == "PRIVMSG" && response[i].recipent == nick && response[i].nick != "") { 
				irc.message(response[i].nick, "Just leave me alone, I know what I'm doing!");
			}
			
			//Someone mentioned bot name on the public channel
			else if(response[i].command == "PRIVMSG") {
				if(response[i].message.compare(0, nick.length(), nick) == 0) {
					string msg = response[i].nick;
					msg.append(": Just leave me alone, I know what I'm doing!");
					irc.message(response[i].recipent, msg);
				}
			}
			/*
			cout << "Data: " << endl;
			cout << "\tcommand: " << response[i].command << endl;
			cout << "\thost: " << response[i].host << endl;
			cout << "\tnick: " << response[i].nick << endl;
			cout << "\trecipent: " << response[i].recipent << endl;
			cout << "\treference: " << response[i].reference << endl;
			cout << "\tmessage: " << response[i].message << endl;
			*/
		}
	}
	cout << "Done!" << endl;
	
	return 0;
}

