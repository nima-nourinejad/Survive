#include "ClientConnection.hpp"

ClientConnection::ClientConnection ()
    : index (-1), fd (-1), status (DISCONNECTED), keepAlive (true){};

std::string readFile (std::string const & path)
{
	std::ifstream file (path.c_str ());
	if (!file.is_open ())
		throw SocketException ("Failed to open file");
	std::stringstream read;
	read << file.rdbuf ();
	file.close ();
	return read.str ();
}

void ClientConnection::createResponseParts ()
{
	status = PREPARINGRESPONSE;
	std::string body = readFile ("map");
	responseParts.push_back (body);
	status = READYTOSEND;
	std::cout << "Response created for client " << index + 1 << std::endl;
}

time_t getCurrentTime ()
{
	time_t current_time = time (nullptr);
	if (current_time == -1)
		throw SocketException ("Failed to get current time");
	return current_time;
}

time_t ClientConnection::getPassedTime () const
{
	time_t current_time = getCurrentTime ();
	if (current_time == -1)
		throw SocketException ("Failed to get passed time");
	return (difftime (current_time, connectTime));
}

void ClientConnection::setCurrentTime ()
{
	connectTime = getCurrentTime ();
}