#pragma once

#include <cstdio>
#include <cstdlib>

#ifdef _DEBUG
#define RELEASE "DEBUG"
#else
#define RELEASE "RELEASE"
#endif

#ifdef _DEBUG // Debug macros

#define C_UNUSED(x) (void)(x)
#define C_ERR(str, ...) fprintf(stderr, "%s:%d:%s() ERROR: " str "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define C_FATAL(str, ...) { fprintf(stderr, "%s:%d:%s() FATAL: " str "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); exit(EXIT_FAILURE); } (void)(0)

#else // Release macros

#define C_UNUSED(x)
#define C_ERR(str, ...) fprintf(stderr, "ERROR: " str "\n", ##__VA_ARGS__)
#define C_FATAL(str, ...) { fprintf(stderr, "FATAL: " str "\n", ##__VA_ARGS__); exit(EXIT_FAILURE); } (void)(0)

#endif