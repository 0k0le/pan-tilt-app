/*
 * Matthew Todd Geiger
 * main.cpp
 */

#include <cstdlib>

#include <macros.hpp>

int main() {
    C_ERR("test");

    if(1)
        C_FATAL("test2");

    return EXIT_SUCCESS;
}