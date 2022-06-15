/*
 * Matthew Todd Geiger
 * macros.hpp
 */

#pragma once

#include <cstdio>
#include <cstdlib>

#ifdef _DEBUG
#define RELEASE "DEBUG"
#else
#define RELEASE "RELEASE"
#endif

#ifdef _DEBUG // Debug macros

#define ONLY_DEBUG(x) x
#define M_UNUSED(x) (void)(x)
#define M_PRINT(str, ...) printf("%s:%d:%s() " str "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define M_ERR(str, ...) fprintf(stderr, "%s:%d:%s() ERROR: " str "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define M_FATAL(str, ...) { fprintf(stderr, "%s:%d:%s() FATAL: " str "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); exit(EXIT_FAILURE); } (void)(0)

#else // Release macros

#define ONLY_DEBUG(x)
#define M_UNUSED(x)
#define M_PRINT(str, ...) printf(str "\n", ##__VA_ARGS__)
#define M_ERR(str, ...) fprintf(stderr, "ERROR: " str "\n", ##__VA_ARGS__)
#define M_FATAL(str, ...) { fprintf(stderr, "FATAL: " str "\n", ##__VA_ARGS__); exit(EXIT_FAILURE); } (void)(0)

#endif