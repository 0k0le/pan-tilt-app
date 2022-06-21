/*
 * Matthew Todd Geiger
 * network.hpp
 */

#include "macros.hpp"

#include <cstring>

#define IRIS_CONTROL "IRIS"
#define ZOOM_CONTROL "ZOOM"
#define FOCUS_CONTROL "FOCU"

class Client {
    public:
        Client(const char* const server);
        ~Client();

};