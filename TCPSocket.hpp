#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <boost/shared_ptr.hpp>
#include "SocketAddr.hpp"
#include "SocketCommon.hpp"

class TCPSocket {
    public:
        ~TCPSocket();
        int Connet(const SocketAddress & addressIn);
        int Bind(const SocketAddress & addressInto);
        int Listen(int backLogIn = 32);
        boost::shared_ptr<TCPSocket> Accept(SocketAddress & addressInFrom);
        int Send(const void* dataIn, int lengthIn);
        int Receive(void* bufferIn, int lengthIn);
    private:
        friend class SocketUtil;
        SOCKET socket;
        TCPSocket (SOCKET socketIn) : socket(socketIn){}
};

typedef boost::shared_ptr<TCPSocket> TCPSocketPtr;

#endif