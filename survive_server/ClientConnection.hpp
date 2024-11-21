#ifndef CLIENTCONNECTION_HPP
#define CLIENTCONNECTION_HPP

#include <ctime>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

#include "SocketException.hpp"

enum ClientStatus
{
	DISCONNECTED,
	WAITFORREQUEST,
	RECEIVINGUNKOWNTYPE,
	RECEIVINGNONCHUNKED,
	RECEIVINGCHUNKED,
	RECEIVED,
	PREPARINGRESPONSE,
	READYTOSEND
};

class ClientConnection
{
	private:

	/// Constants
		const size_t MAX_HEADER_SIZE = 32768;
	
      public:

	/// Public Attributes
	int index;
	int fd;
	int status;
	bool keepAlive;
	time_t connectTime;
	std::string request;
	std::vector<std::string> responseParts;
	size_t maxBodySize;

	/// Public Methods
	ClientConnection ();
	void createResponseParts ();
	time_t getPassedTime () const;
	void setCurrentTime ();


};

#endif