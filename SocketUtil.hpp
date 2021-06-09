#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

#include <iostream>
#include <errno.h>
#include <vector>
#include "boost/shared_ptr.hpp"
#include "UDPSocket.hpp"
#include "TCPSocket.hpp"

enum SocketAddressFamily {
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketUtil {
	public:
		static UDPSocketPtr CreateUDPSocket(SocketAddressFamily familyIn);
		static TCPSocketPtr CreateTCPSocket(SocketAddressFamily familyIn);
		static void ReportError(std::string msg);
		static int GetLastError();
		static fd_set* FillSetFromVector(fd_set & setOut, const std::vector<TCPSocketPtr>* socketIn);
		static void FillVectorFromSet(std::vector<TCPSocketPtr>* socketsOut, const std::vector<TCPSocketPtr>* socketIn, const fd_set & setIn);
		static int Select(const std::vector<TCPSocketPtr> * readSetIn, std::vector<TCPSocketPtr> * readSetOut, 
		const std::vector<TCPSocketPtr> * writeSetIn, std::vector<TCPSocketPtr> * writeSetOut,
		const std::vector<TCPSocketPtr> * exceptSetIn, std::vector<TCPSocketPtr> * exceptSetOut);
};

#endif