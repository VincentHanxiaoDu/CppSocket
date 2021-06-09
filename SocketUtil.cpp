#include <boost/shared_ptr.hpp>
#include <sys/socket.h>
#include <vector>
#include "SocketUtil.hpp"
#include "TCPSocket.hpp"

UDPSocketPtr SocketUtil::CreateUDPSocket(SocketAddressFamily familyIn) {
    int sock = socket(familyIn, SOCK_DGRAM, IPPROTO_UDP);
    if (sock != -1) {
        return UDPSocketPtr(new UDPSocket(sock));
    } else {
        ReportError("SocketUtil::CreateUDPSocket");
        return nullptr;
    }
}


TCPSocketPtr SocketUtil::CreateTCPSocket(SocketAddressFamily familyIn) {
    int sock = socket(familyIn, SOCK_STREAM, IPPROTO_TCP);
    if (sock != -1) {
        return TCPSocketPtr(new TCPSocket(sock));
    } else {
        ReportError("SocketUtil::CreateTCPSocket");
        return nullptr;
    }
}

void SocketUtil::ReportError(std::string msg) {
    std::cout << "ERROR: " << msg << std::endl;
}

int SocketUtil::GetLastError() {
    return errno;
}

fd_set* SocketUtil::FillSetFromVector(fd_set & setOut, const std::vector<TCPSocketPtr>* socketsIn) {
    if (socketsIn) {
        FD_ZERO(& setOut);
        for (const TCPSocketPtr & socketi : *socketsIn) {
            FD_SET(socketi->socket, & setOut);
        }
        return &setOut;
    } else {
        return nullptr;
    }
}

void SocketUtil::FillVectorFromSet(std::vector<TCPSocketPtr> * socketsOut, const std::vector<TCPSocketPtr>* socketsIn, const fd_set & setIn) {
    if (socketsIn && socketsOut) {
        socketsOut->clear();
        for (const TCPSocketPtr & socketi : * socketsIn) {
            if (FD_ISSET(socketi->socket, & setIn)) {
                socketsOut->push_back(socketi);
            }
        }
    }
}

int SocketUtil::Select(const std::vector<TCPSocketPtr> * readSetIn, std::vector<TCPSocketPtr> * readSetOut, 
const std::vector<TCPSocketPtr> * writeSetIn, std::vector<TCPSocketPtr> * writeSetOut,
const std::vector<TCPSocketPtr> * exceptSetIn, std::vector<TCPSocketPtr> * exceptSetOut) {
    fd_set read, write, except;

    fd_set *readPtr = FillSetFromVector(read, readSetIn);
    fd_set *writePtr = FillSetFromVector(write, writeSetIn);
    fd_set *exceptPtr = FillSetFromVector(except, exceptSetIn);

    int result = select(0, readPtr, writePtr, exceptPtr, nullptr);
    if (result > 0) {
        FillVectorFromSet(readSetOut, readSetIn, read);
        FillVectorFromSet(writeSetOut, writeSetIn, write);
        FillVectorFromSet(exceptSetOut, exceptSetIn, except);
    }
    return result;
}