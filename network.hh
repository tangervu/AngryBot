/**
 * A class for establishing connection into a server and transferring data using sockets
 *
 * @author Tuomas Angervuori <tuomas.angervuori@gmail.com>
 */

#ifndef NETWORK_HH
#define NETWORK_HH

#include <string>
#include <stdexcept>

using namespace std;

namespace network {
	
	static const unsigned int MAX_DATA_SIZE = 10240;
	static const bool NET_DEBUG = false;
	static const unsigned int NET_TIMEOUT = 500000; //0.5 seconds
	
	class Network {
		
		private:
		int sock;
		
		public:
		
		/**
		 * Constructor, establishing a connection to a server
		 * @param hostname Hostname or ip address of the server
		 * @param port Port of the service
		 * @throw runtime_error Connection failed
		 */
		Network(const string hostname, const int port) throw (runtime_error);
		Network(const Network& net);
		Network &operator=(const Network &network);
		
		/**
		 * Read data from socket
		 * @note The string read from socket may be partial
		 * @param timeout Timeout for data read in microseconds, 0 = no limits
		 * @return Data recieved from the socket
		 * @throw runtime_error Connection failure
		 * @throw logic_error Unable to process the response
		 */
		string read(const int timeout) throw (runtime_error, logic_error);
		string read() throw (logic_error);
		
		/**
		 * Write data into socket
		 * @throw runtime_error Connection failure
		 * @throw logic_error Unable to process the data
		 */
		void write(const string data) throw (logic_error);
		
		/**
		 * Destructor, closing the socket connections
		 */
		~Network();
		
	};
}

#endif
