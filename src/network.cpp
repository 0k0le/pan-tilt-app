/*
 * Matthew Todd Geiger
 * network.cpp
 */

#include "network.hpp"

Client::Client(const char* const server, const int portnum,
    const int packetLen) : _port(portnum), _packetLen(packetLen) {
    char testmsg[PKTSIZE] = "TEST";

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
}

Client::~Client() {
    M_PRINT("Killing connection with server");
    close(_sock);
}