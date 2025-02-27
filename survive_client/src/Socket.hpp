#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <cerrno>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

class Socket
{
      protected:
	constexpr bool acceptableError (int error) const
	{
		return (error == EAGAIN || error == EWOULDBLOCK);
	}
	class ClientConnection
	{
		public:
			int fd;
			bool connected;
			bool sent;
			bool received;
			std::string message;
			ClientConnection () : fd (-1), connected (false), sent (false), received (false){};
	};
	class SocketException : public std::runtime_error
	{
	      public:
			SocketException (std::string const & message, Socket * socket)
				: std::runtime_error (message)
			{
				if (socket != nullptr)
					socket->closeSocket ();
			};
	};
	int _socket_fd;
	struct sockaddr_in _address;
	const int _port;
	std::string const _name;

      public:
	Socket (int port, std::string const & name);
	void setAddress ();
	void createSocket ();

	virtual void connectToSocket () = 0;
	virtual void closeSocket () = 0;
	int getSocketFD () const;
	void makeSocketNonBlocking ();
	std::string getName() const;
	void customSignal();
	static void signalHandler(int signal);
	static volatile sig_atomic_t signal_status;
};

#endif