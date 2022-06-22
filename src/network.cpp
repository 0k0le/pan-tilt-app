/*
 * Matthew Todd Geiger
 * network.cpp
 */

#include "network.hpp"

int recv_full(int sock, int size, char* buf) {
    int remainder = 0;
    while(remainder = recv(sock, buf, size, 0) != 0) {
        size -= size - remainder;
    }
}

Client::Client(const char* const server, const int portnum,
    const int packetLen) : _port(portnum), _packetLen(packetLen) {
    //char testmsg[PKTSIZE] = "TEST";
    char* testmsg = new char[_packetLen];
    strcpy(testmsg, "test");

    struct sockaddr_in servaddr;

    _sock = socket(AF_INET, SOCK_STREAM, 0);
    if(_sock == -1)
        M_FATAL("Failed to open TCP socket");

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server);
    servaddr.sin_port = htons(_port);
    
    if(connect(_sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
        M_FATAL("Failed to connect to BBG @ IP %s", server);

    send(_sock, testmsg, _packetLen, 0);
    recv(_sock, testmsg, _packetLen, 0);

    M_PRINT("Recieved: %s", testmsg);
    if(strcmp(testmsg, "conf") != 0)
        M_FATAL("Did not recieve confirmation from server");

    delete testmsg;
}

void Client::RequestChange(const char* const command, int value) {
    char* recvbuf = new char[_packetLen];
    memset(recvbuf, 0, _packetLen);
    M_PRINT("Sending command: %s", command);

    send(_sock, command, _packetLen, 0);
    send(_sock, &value, sizeof(int), 0);

    recv(_sock, recvbuf, _packetLen, 0);

    M_PRINT("Server Response: %s", recvbuf);

    if(memcmp(recvbuf, SUCCESS, 4) != 0)
        M_FATAL("Failed to get valid response from server");

    delete recvbuf;
}

Client::~Client() {
    M_PRINT("Killing connection with server");
    close(_sock);
}