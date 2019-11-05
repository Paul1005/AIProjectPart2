/////////////////////////////////////////////////////////////////////////////////
//
// Sample OpenCL application
// (c) 2015 Borna Noureddin
// British Columbia Institute of Technology
// Adapted from: OpenCL(R) Programming Guide
//      Authors: Aaftab Munshi, Benedict Gaster, Timothy Mattson, James Fung, Dan Ginsburg
//
/////////////////////////////////////////////////////////////////////////////////

#include "Main.h"
#include "Serial.h"
#include "OpenCL.h"

#ifndef __APPLE__
/*
 * timezone information is stored outside the kernel so tzp isn't used anymore.
 *
 * Note: this function is not for Win32 high precision timing purpose. See
 * elapsed_time().
 */
int gettimeofday(struct timeval* tp, struct timezone* tzp)
{
	FILETIME    file_time;
	SYSTEMTIME  system_time;
	ULARGE_INTEGER ularge;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;

	tp->tv_sec = (long)((ularge.QuadPart - epoch) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);

	return 0;
}
#endif

//	main() for HelloWorld example
int main(int argc, char** argv)
{
	std::cout << "Serially ran in " << SerialDemo() << " microseconds" << std::endl << std::endl;

	std::cout << "OpenCL ran in " << OpenCLDemo() << " microseconds" << std::endl << std::endl;

	std::cout << std::endl << std::endl;
	std::cout << "Executed program succesfully." << std::endl;

	return 0;
}