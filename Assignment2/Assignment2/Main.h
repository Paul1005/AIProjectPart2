#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <stack> 
#include "Cell.h"
#include <vector>
#include <cmath>    
#include <limits.h>
#include <chrono>

#ifdef __APPLE__
#include <sys/time.h>
#else
#include <Windows.h>

/* FILETIME of Jan 1 1970 00:00:00. */
static const unsigned __int64 epoch = ((unsigned __int64)116444736000000000ULL);

int gettimeofday(struct timeval* tp, struct timezone* tzp);
#endif

using namespace std;

//  Constants
extern const int ARRAY_SIZE;
extern int array_size;
