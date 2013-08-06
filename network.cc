/**
 * A class for establishing connection into a server and transferring data using sockets
 *
 * @author Tuomas Angervuori <tuomas.angervuori@gmail.com>
 */

#include <string>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntoa()
#include <netdb.h> //gethostbyname
#include <iostream>
#include <istream>
#include <ostream>

#include "network.hh"

using namespace std;

namespace network {
	
	Network::Network(const string hostname, const int port) throw (runtime_error) {
		if(NET_DEBUG) {
			cout << "[NET DEBUG]: hostname " << hostname << endl;
		}
			
		//Get hosts ip address
		struct hostent *h;
		h = gethostbyname(hostname.c_str());
		
		if(NET_DEBUG) {
			cout << "[NET DEBUG]: hostname ip: " << inet_ntoa(*((struct in_addr *)h->h_addr)) << endl;
		}
		const char *addr = inet_ntoa(*((struct in_addr *)h->h_addr));
		
		struct sockaddr_in sa;
		
		if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)  {
			throw runtime_error("Creating socket failed");
		}
		
		sa.sin_family = AF_INET;
		sa.sin_port = htons(port);
		sa.sin_addr.s_addr = inet_addr(addr);
		
		if(NET_DEBUG) {
			cout << "[NET DEBUG]: connecting to: " << hostname << " (" << port << ")" << endl;
		}
		if(connect(sock,(struct sockaddr *)&sa, sizeof(sa)) < 0) {
			throw runtime_error("Connecting to address failed");
		}
	}
	
	Network::Network(const Network& net) {
		
		this->sock = net.sock;
	}
	
	Network &Network::operator=(const Network& net) {
		if(&net==this) {
			return *this;
		}
		this->sock = sock;
		return *this;
	}
	
	string Network::read(const int timeout) throw (runtime_error, logic_error) {
		char buffer[MAX_DATA_SIZE];
		string response;
		int response_length;
		fd_set read_fds;
		
		if(NET_DEBUG) {
			cout << "[NET DEBUG]: reading data (timeout " << timeout << ")" << endl;
		}
		
		if(timeout >= 0) {
			//TODO check if this is really working...
			struct timeval tv;
			tv.tv_sec=0; //seconds
			tv.tv_usec=timeout; //microseconds
			
			FD_ZERO(&read_fds);
			FD_SET(sock, &read_fds);
			select(sock+1, &read_fds, NULL, NULL, &tv);
		}
		
		if((timeout < 0) || FD_ISSET(sock, &read_fds)) {
			response_length = recv(sock, buffer, MAX_DATA_SIZE-1, 0);
			if(response_length == -1) {
				throw logic_error("Data could not be read");
			}
		}
		
		//No data recieved
		else {
			if(NET_DEBUG) {
				cout << "[NET DEBUG]: read data timeout" << endl;
			}
			return "";
		}
		
		if(NET_DEBUG) {
			cout << "[NET DEBUG]: read data: \"" << buffer << "\" (length " << response_length << ")" << endl;
		}
		if(response_length == 0) {
			throw runtime_error("Connection closed");
		}
		buffer[response_length] = '\0';
	
		response = buffer;
		return response;
	}
	
	string Network::read() throw (logic_error) {
		return this->read(NET_TIMEOUT);
	}
	
	void Network::write(const string data) throw (logic_error) {
		int data_length;
		
		if(data.length() >= MAX_DATA_SIZE) {
			throw logic_error("Output data too big");
		}
		
		const char *char_data = data.c_str();
		data_length = data.length();
		if(NET_DEBUG) {
			cout << "[NET DEBUG]: writing data: \"" << char_data << "\" (length: " << data_length << ")" << endl;
		}
		
		if(send(sock, char_data, data_length, 0) == 0) {
			throw runtime_error("Connection closed");
		}
	}
	
	Network::~Network() {
		if(NET_DEBUG) {
			cout << "[NET DEBUG]: closing connection" << endl;
		}
		shutdown(sock,SHUT_RDWR);
	}
}
