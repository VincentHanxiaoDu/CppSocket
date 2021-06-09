#include "TCPSocket.hpp"
#include "SocketUtil.hpp"

int TCPSocket::Connet(const SocketAddress & addressIn) {
    int error = connect(socket, &addressIn.socketAddr, addressIn.GetSize());
    if (error < 0) {
        SocketUtil::ReportError("TCPSocket::Connect");
        return -SocketUtil::GetLastError();
    } else {
        int info = error;
        return info;
    }
}

int TCPSocket::Bind(const SocketAddress & addressInBind) {
	int error = bind(socket, & addressInBind.socketAddr, addressInBind.GetSize());
	if (error != 0) {
		SocketUtil::ReportError("TCPSocket::Bind");
		return -SocketUtil::GetLastError();
	} else {
		return 0;
	}
}

int TCPSocket::Listen(int backLogIn) {
    int error = listen(socket, backLogIn);
    if (error < 0) {
        SocketUtil::ReportError("TCPSocket::Listen");
        return -SocketUtil::GetLastError();
    } else {
        int info = error;
        return info;
    }
}

TCPSocketPtr TCPSocket::Accept(SocketAddress &addressInFrom) {
    socklen_t length = addressInFrom.GetSize();
    int newSocket = accept(socket, & addressInFrom.socketAddr, &length);

    if (newSocket != -1) {
        return TCPSocketPtr(new TCPSocket(newSocket));
    } else {
        SocketUtil::ReportError("TCPSocket::Accept");
        return nullptr;
    }
}

int TCPSocket::Send(const void* dataIn, int lengthIn) {
    int byteSentLength = send(socket, dataIn, lengthIn, 0);

    if (byteSentLength < 0) {
        SocketUtil::ReportError("TCPSocket::Send");
        return -SocketUtil::GetLastError();
    } else {
        return byteSentLength;
    }
}

int TCPSocket::Receive(void* bufferIn, int lengthIn) {
    int byteReceivedLength = recv(socket, bufferIn, lengthIn, 0);

    if (byteReceivedLength < 0) {
        SocketUtil::ReportError("TCPSocket::TCPSocket");
        return -SocketUtil::GetLastError();
    } else {
        byteReceivedLength;
    }
}

TCPSocket::~TCPSocket() {
	shutdown(socket, SHUT_RDWR);
}