#ifndef SOCKETADDR_H
#define SOCKETADDR_H

#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/shared_ptr.hpp>

// IPV4 Socket Address
class SocketAddress {
	public:
		SocketAddress (uint32_t addressIn, uint16_t portIn);
		SocketAddress (const char* addressIn, uint16_t portIn);
		SocketAddress (const sockaddr& sockAddrIn);
	private:
		sockaddr socketAddr;
		friend class UDPSocket;
		friend class TCPSocket;
		sockaddr_in* GetAsSocketAddrIn();
		int GetSize() const;
};

typedef boost::shared_ptr<SocketAddress> SocketAddressPtr;

class SocketAddressFactory {
    static SocketAddressPtr CreateIPv4FromStr(const std::string& stringIn);
};

#endif