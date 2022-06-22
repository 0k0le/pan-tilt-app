/*
 * Matthew Todd Geiger
 * network.hpp
 */

#pragma once

#include "macros.hpp"

#include <cstring>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>

#define IRIS_CONTROL "IRIS"
#define ZOOM_CONTROL "ZOOM"
#define FOCUS_CONTROL "FOCU"

#define SUCCESS "conf"

#define PORTNO  65432
#define PKTSIZE 5

class Client {
    public:
        Client(const char* const server, const int portnum = PORTNO,
            const int packetLen = PKTSIZE);
        ~Client();

        void RequestChange(const char* const command, int value);

    private:
        int _sock;
        int _port;
        int _packetLen;

};