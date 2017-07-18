#ifndef cocs_util_h
#define cocs_util_h

#include "cocos2d.h"

// Error handling function
// status - true if init worked, false otherwise
#define HANDLE_INIT_ERRORS(status, ...)\
    if (!status) \
    { \
    	log(__VA_ARGS__); \
    	return false; \
    }

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

#endif