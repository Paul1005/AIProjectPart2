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

struct timerResult_t
{
	int seconds;
	int useconds;
};


// Helper class for timing calculations
class CTiming
{
public:
	CTiming() {}
	~CTiming() {}

	void Start() { gettimeofday(&tvBegin, NULL); }
	void End() { gettimeofday(&tvEnd, NULL); }
	bool Diff(int& seconds, int& useconds)
	{
		long int diff = (tvEnd.tv_usec + 1000000 * tvEnd.tv_sec) -
			(tvBegin.tv_usec + 1000000 * tvBegin.tv_sec);
		seconds = diff / 1000000;
		useconds = diff % 1000000;
		return (diff < 0) ? true : false;
	}

private:
	struct timeval tvBegin, tvEnd, tvDiff;

};
