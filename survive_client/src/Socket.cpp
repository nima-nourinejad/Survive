#include "Socket.hpp"

Socket::Socket (int port, std::string const & name)
    : _port (port), _name (name)
{
	createSocket ();
	setAddress ();
}

void Socket::createSocket ()
{
	_socket_fd = socket (AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
		throw SocketException ("Failed to create socket", nullptr);
}

void Socket::setAddress ()
{
	struct addrinfo hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	struct addrinfo * result = nullptr;
	std::string host = "127.0.0.3";
	int error = getaddrinfo (host.c_str (), nullptr, &hints, &result);
	_address = *reinterpret_cast<struct sockaddr_in *> (result->ai_addr);
	freeaddrinfo (result);
	_address.sin_port = htons (9001);
}

int Socket::getSocketFD () const
{
	return _socket_fd;
}

void Socket::makeSocketNonBlocking ()
{
	if(signal_status == SIGINT)
		return;
	int currentFlags = fcntl (_socket_fd, F_GETFL, 0);
	if (currentFlags == -1)
		throw SocketException ("Failed to get socket flags", this);
	int newFlags = currentFlags | O_NONBLOCK;
	if (fcntl (_socket_fd, F_SETFL, newFlags) == -1)
		throw SocketException ("Failed to set socket flags", this);
}

void Socket::signalHandler (int signal)
{
	if (signal == SIGINT)
		signal_status = SIGINT;
}

void Socket::customSignal ()
{
	if (signal (SIGINT, &signalHandler) == SIG_ERR)
		throw SocketException ("Failed to set signal handler", this);
}

volatile sig_atomic_t Socket::signal_status = 0;

std::string Socket::getName() const
{
	return _name;
};
