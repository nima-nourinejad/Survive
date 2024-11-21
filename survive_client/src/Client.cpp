#include "Client.hpp"

Client::Client (int port, std::string const & name)
    : Socket (port, name){};

void Client::connectToSocket ()
{
	if (_client.connected || signal_status == SIGINT)
		return;
	if (connect (_socket_fd, (struct sockaddr *)&_address, sizeof (_address)) == -1)
	{
		if (errno != EINPROGRESS && errno != ECONNREFUSED)
			throw SocketException ("Failed to connect to socket", this);
	}
	else
	{
		std::cout << _name << " connected to server" << std::endl;
		_client.connected = true;
		char buffer[16384] = {};
		ssize_t bytes_received;
		bytes_received = recv (_socket_fd, buffer, sizeof (buffer), 0);
		if (bytes_received == -1)
		{
			if (!acceptableError (errno))
				throw SocketException ("Failed to receive message", this);
		}
		else if (bytes_received == 0)
		{
			_client.connected = false;
			_client.sent = false;
			_client.received = false;
			_client.message.clear ();
			_client.fd = -1;
			std::cout << "Connection closed by server" << std::endl;
		}
		else
		{
			_client.sent = false;
			_client.received = true;
			_client.message = buffer;
			
			std::cout << "I received the map from server" << std::endl;
			std::fstream file("map", std::ios::out | std::ios::trunc);
			file << _client.message;
			file.close();
		}
	}
};

void Client::closeSocket ()
{
	std::cout << std::endl << _name << " is shutting down" << std::endl;
	signal (SIGINT, SIG_DFL);
	_client.connected = false;
	_client.sent = false;
	_client.received = false;
	_client.message.clear ();
	_client.fd = -1;
	close (_socket_fd);
};

void Client::receiveMessage ()
{
	if (!_client.connected || !_client.sent || _client.received || signal_status == SIGINT)
		return;
	char buffer[16384] = {};
	ssize_t bytes_received;
	bytes_received = recv (_socket_fd, buffer, sizeof (buffer), 0);
	if (bytes_received == -1)
	{
		if (!acceptableError (errno))
			throw SocketException ("Failed to receive message", this);
	}
	else if (bytes_received == 0)
	{
		_client.connected = false;
		_client.sent = false;
		_client.received = false;
		_client.message.clear ();
		_client.fd = -1;
		std::cout << "Connection closed by server" << std::endl;
	}
	else
	{
		_client.sent = false;
		_client.received = true;
		_client.message = buffer;

		std::cout << "I received this message from server:" << _client.message << std::endl; 
	}
};

std::string Client::getMessage () const
{
	return _client.message;
};

bool Client::isConnected () const
{
	return _client.connected;
};

bool Client::isReceived () const
{
	return _client.received;
};

bool Client::isSent () const
{
	return _client.sent;
};