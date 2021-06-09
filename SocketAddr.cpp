#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <boost/make_shared.hpp>
#include "SocketAddr.hpp"

// SocketAddress
SocketAddress::SocketAddress (uint32_t addressIn, uint16_t portIn) {
	sockaddr_in* socketAddrIn = GetAsSocketAddrIn();
	socketAddrIn->sin_family = AF_INET;
	socketAddrIn->sin_addr.s_addr = htonl(addressIn);
	socketAddrIn->sin_port = htons(portIn);
}

SocketAddress::SocketAddress (const char* addressIn, uint16_t portIn) {
	sockaddr_in* socketAddrIn = GetAsSocketAddrIn();
	socketAddrIn->sin_family = AF_INET;
	inet_pton(AF_INET, addressIn, &socketAddrIn->sin_addr);
	socketAddrIn->sin_port = htons(portIn);
}

SocketAddress::SocketAddress (const sockaddr& sockAddrIn) {
	memcpy(& socketAddr, & sockAddrIn, sizeof(sockaddr));
}

sockaddr_in* SocketAddress::GetAsSocketAddrIn() {
	return reinterpret_cast<sockaddr_in*>(& socketAddr);
}

int SocketAddress::GetSize() const{
	return sizeof(socketAddr);
}


SocketAddressPtr SocketAddressFactory::CreateIPv4FromStr(const std::string& stringIn) {
    auto pos = stringIn.find_last_of(':');
    std::string host, service;
    if (pos != std::string::npos) {
        host = stringIn.substr(0, pos);
        service = stringIn.substr(pos+1);
    } else {
        host = stringIn;
        service = "0";    // Use default port
    }
    
    addrinfo hint;
    memset(& hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    
    addrinfo* result;
    int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
    if (error !=0 && result != nullptr) {    // error
        freeaddrinfo(result);
        return nullptr;
    }
    
    while (!result->ai_addr && result->ai_next) {    // find the reuslt
        result = result->ai_next;
    }
    
    if (!result->ai_addr) {    // no result found
        freeaddrinfo(result);
        return nullptr;
    }
    
    auto addr = boost::make_shared<SocketAddress>(*result->ai_addr);
    freeaddrinfo(result);
    return addr;
}
