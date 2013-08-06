/**
 * A class for implementing the IRC protocol
 *
 * @author Tuomas Angervuori <tuomas.angervuori@gmail.com>
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>

#include "irc.hh"
#include "network.hh"
#include "tools.hh"

using namespace std;

namespace irc {
	
	/**
	 * Constructor: connect to a irc server
	 * 
	 * @param hostname Hostname of the server
	 * @param port Port
	 */
	Irc::Irc(string hostname, int port) {
		if(IRC_DEBUG) {
			cout << "[IRC DEBUG]: Connecting to " << hostname << ":" << port << endl;
		}
		this->conn = new network::Network(hostname, port);
		this->buffer = "";
	}
	
	Irc::Irc(string hostname) {
		if(IRC_DEBUG) {
			cout << "[IRC DEBUG]: Connecting to " << hostname << ":" << IRC_PORT << endl;
		}
		this->conn = new network::Network(hostname, IRC_PORT);
		this->buffer = "";
	}
	
	/**
	 * Destructor: log out from the irc server
	 */
	Irc::~Irc() {
		if(IRC_DEBUG) {
			cout << "[IRC DEBUG]: Terminating connection" << endl;
		}
		try {
			this->quit("Connection terminated");
		}
		catch (...) {
			//No exceptions from destructor...
		}
		delete this->conn;
	}
	
	void Irc::login(const string username, const string realname) {
		string data = "USER " + username + " 0 0 :" + realname;
		this->putData(data);
	}
	
	void Irc::nick(const string nick) {
		string data = "NICK " + nick;
		this->putData(data);
	}
	
	vector<response> Irc::getData(const int timeout) {
		
		if(IRC_DEBUG) {
			cout << "[IRC DEBUG]: Waiting for data (timeout " << timeout << ")" << endl;
		}
		
		vector<response> result;
		vector<string> rows;
		
		//Read unprocessed data from buffer
		string data = this->buffer;
		if(IRC_DEBUG) {
			cout << "[IRC DEBUG]: buffer status: " << data << endl;
		}
		
		//Read data from socket
		data.append(this->conn->read(timeout));
		
		//Split rows into vector
		rows = tools::split("\n",data);
		
		//Check if last row is partial
		bool isPartial;
		if(data.at(data.length()-1) != '\n') {
			isPartial = true;
		}
		else {
			isPartial = false;
			this->buffer = "";
		}
		
		unsigned int lastRowNum = rows.size() - 1;
		for(vector<string>::size_type i=0; i<lastRowNum; i++) {
			
			//Last row was partial, store it into buffer for later use
			if(i == lastRowNum && isPartial) {
				this->buffer = rows[i];
			}
			
			else {
				if(IRC_PRINT_MESSAGES) {
					cout << "<- " << rows[i] << endl;
				}
			
				response tempdata;
				vector<string> row_elements;
				
				//Parse message
				row_elements = tools::split(":",rows[i],3);
				if(row_elements.size() >= 3) {
					tempdata.message = row_elements[2];
				}
				
				//Automatically answer to PING (has different message format)
				if(row_elements[0] == "PING ") {
					this->pong(row_elements[1]);
				}
				
				else {
					vector<string> parameters;
					vector<string> userdata;
					
					//Clean up extra space from the end
					row_elements[1].erase(row_elements[1].size()-1,row_elements[1].size());
					
					parameters = tools::split(" ",row_elements[1],4);
					
					if(parameters.size() < 2) {
						cout << "size: " << parameters.size() << endl;
						throw logic_error("Don't know how to handle parameters: " + rows[i]);
					}
					
					userdata = tools::split("!",parameters[0],2);
					if(userdata.size() == 1) {
						tempdata.host = userdata[0];
					}
					else if(userdata.size() == 2) {
						tempdata.nick = userdata[0];
						tempdata.host = userdata[1];
					}
					else {
						throw logic_error("Don't know how to handle user data " + rows[i]);
					}
					
					tempdata.command = parameters[1];
					
					if(parameters.size() >= 3) {
						tempdata.recipent = parameters[2];
					}
					
					if(parameters.size() >= 4) {
						tempdata.reference = parameters[3];
					}
					
					result.push_back(tempdata);
				}
			}
		}
		
		return result;
		
	}
	
	vector<response> Irc::getData() {
		return this->getData(IRC_TIMEOUT);
	}
	

	vector<response> Irc::waitForData() {
		return this->getData(-1);
	}
	
	void Irc::putData(const string data) {
		if(IRC_DEBUG) {
			cout << "[IRC DEBUG]: Writing data" << endl;
		}
		if(IRC_PRINT_MESSAGES) {
			cout << "-> " << data << endl;
		}
		string putData = data + "\n";
		this->conn->write(putData);
	}
	
	void Irc::join(const string channel) {
		string data = "JOIN " + channel;
		this->putData(data);
	}
	
	void Irc::leave(const string channel, const string message) {
		string data = "PART " + channel + " :" + message;
		this->putData(data);
	}
	
	void Irc::message(const string recipent, const string message) {
		string data = "PRIVMSG " + recipent + " :" + message;
		this->putData(data);
	}
	
	void Irc::giveOps(const string channel, const string user) {
		string data = "MODE " + channel + " +o " + user;
		this->putData(data);
	}
	
	void Irc::removeOps(const string channel, const string user) {
		string data = "MODE " + channel + " -o " + user;
		this->putData(data);
	}
	
	void Irc::kick(const string channel, const string user, const string message) {
		string data = "KICK " + channel + " " + user + " :" + message;
		this->putData(data);
	}
	
	void Irc::ban(const string channel, const string mask) {
		string data = "MODE " + channel + " +b " + mask;
		this->putData(data);
	}
	
	void Irc::unban(const string channel, const string mask) {
		string data = "MODE " + channel + " -b " + mask;
		this->putData(data);
	}

	void Irc::invite(const string channel, const string user) {
		string data = "INVITE " + user + " " + channel;
		this->putData(data);
	}
	
	void Irc::topic(const string channel, const string topic) {
		string data = "TOPIC " + channel + " :" + topic;
		this->putData(data);
	}
	
	void Irc::pong(const string response) {
		string data = "PONG :" + response;
		this->putData(data);
	}
	
	void Irc::quit(const string message) {
		string data = "QUIT :" + message;
		this->putData(data);
	}
}
