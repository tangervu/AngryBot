/**
 * A class for implementing the IRC protocol
 *
 * @author Tuomas Angervuori <tuomas.angervuori@gmail.com>
 */
#ifndef IRC_HH
#define IRC_HH

#include <string>
#include <vector>
#include "network.hh"

#define IRC_PORT 6667
#define IRC_DEBUG false
#define IRC_PRINT_MESSAGES true
#define IRC_TIMEOUT 500000

using namespace std;

namespace irc {
	
	/**
	 * @brief Data structured transmitted to and from the irc server
	 */
	struct response {
		string command;
		string host;
		string nick;
		string recipent;
		string reference;
		string message;
	};
	
	class Irc {
		
		private:
		network::Network *conn;
		string buffer; //Place to store unprocessed data
	
		
		public:
		
		/**
		 * Constructor, connect to a IRC server
		 * @param hostname Hostname for the IRC server
		 * @param port Connection port for the IRC server
		 */
		Irc(const string hostname, const int port);
		Irc(const string hostname);
		
		/**
		 * Destructor, close the connection from the irc server
		 */
		~Irc();
		
		/**
		 * Login into the IRC server
		 * @param username Username
		 * @param realname Full name
		 */
		void login(const string username, const string realname);
		
		/**
		 * Set nickname for user
		 * @param nick Nickname text
		 */
		void nick(const string nick);
		
		/**
		 * Read data from the IRC server
		 * @return Data read from the IRC server
		 * @note Return data may be empty!
		 */
		vector<response> getData();
		
		/**
		 * Read data from the IRC server
		 * @param timeout Timeout for recieving data
		 * @return Data read from the IRC server
		 * @note Return data may be empty!
		 */
		vector<response> getData(const int timeout);
		
		/**
		 * Wait for input from the IRC server
		 * @return Data read from the IRC server
		 * @note Return data may be empty!
		 */
		vector<response> waitForData();
		
		/**
		 * Send data to the IRC server
		 * @param data IRC command string to be sent to the server
		 * @return false if response is ERROR
		 */
		void putData(const string data);
		
		/**
		 * Join a channel
		 * @param channel Name of the channel
		 */
		void join(const string channel);
		
		/**
		 * Leave a channel
		 * @param channel Name of the channel
		 * @param message Message text
		 */
		void leave(const string channel, const string message);
		
		/**
		 * Send a message
		 * @param recipent Name of the channel or user nick 
		 * @param message Message text
		 */
		void message(const string recipent, const string message);
		
		/**
		 * Give ops to a user on a channel
		 * @param channel Name of the channel
		 * @param user User nick
		 */
		void giveOps(const string channel, const string user);
		
		/**
		 * Remove ops from user on a channel
		 * @param channel Name of the channel
		 * @param user User nick
		 */
		void removeOps(const string channel, const string user);
		
		/**
		 * Kick user from a channel
		 * @param channel Name of the channel
		 * @param user User nick
		 * @param message Message text
		 */
		void kick(const string channel, const string user, const string message);
		
		/**
		 * Ban user from accessing the channel
		 * @param channel Name of the channel
		 * @param mask Mask used for banning
		 */
		void ban(const string channel, const string mask);
		
		/**
		 * Remove ban for user on a channel
		 * @param channel Name of the channel
		 * @param mask Mask used for banning
		 */
		void unban(const string channel, const string mask);
		
		/**
		 * Invite user to a channel
		 * @param channel Name of the channel
		 * @param user User nick
		 */
		void invite(const string channel, const string user);
		
		/**
		 * Set channel topic
		 * @param channel Name of the channel
		 * @param topic Topic message
		 */
		void topic(const string channel, const string topic);
		
		/**
		 * Respond to PING
		 * @param response Reply message
		 */
		void pong(const string response);
		
		/**
		 * Quit from the server
		 * @param message Exit message
		 */
		void quit(const string message);
	};
}

#endif
