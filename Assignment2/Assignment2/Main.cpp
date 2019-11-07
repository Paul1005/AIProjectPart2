#include "Serial.h"
#include "OpenCL.h"
#include <iostream>

int main(int argc, char** argv)
{
	long serialRunTime;
	long openCLCPURunTime, openCLGPURunTime, openCLCPUGPURunTime;
	try
	{
		std::cout << "Running algorithm serially..." << std::endl;
		serialRunTime = Serial::SerialDemo();
		std::cout << "Serially ran in " << serialRunTime << " nanoseconds" << std::endl << std::endl;
	}
	catch (std::runtime_error ex)
	{
		std::cerr << "Serial: " << ex.what() << std::endl;
	}

	try
	{
		std::cout << "Running algorithm on OpenCL CPU..." << std::endl;
		openCLCPURunTime = OpenCLDemo(CL_DEVICE_TYPE_CPU);
		std::cout << "OpenCL ran in " << openCLCPURunTime << " nanoseconds" << std::endl << std::endl;
	}
	catch (std::runtime_error ex)
	{
		std::cerr << "OpenCL CPU: " << ex.what() << std::endl;
	}

	try
	{
		std::cout << "Running algorithm on OpenCL GPU..." << std::endl;
		openCLGPURunTime = OpenCLDemo(CL_DEVICE_TYPE_GPU);
		std::cout << "OpenCL ran in " << openCLGPURunTime << " nanoseconds" << std::endl << std::endl;
	}
	catch (std::runtime_error ex)
	{
		std::cerr << "OpenCL GPU: " << ex.what() << std::endl;
	}

	try
	{
		std::cout << "Running algorithm on OpenCL CPU+GPU..." << std::endl;
		openCLCPUGPURunTime = OpenCLDemo(CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_GPU);
		std::cout << "OpenCL ran in " << openCLCPUGPURunTime << " nanoseconds" << std::endl << std::endl;
	}
	catch (std::runtime_error ex)
	{
		std::cerr << "OpenCL CPU+GPU: " << ex.what() << std::endl;
	}

	std::cout << std::endl << std::endl;
	std::cout << "Executed program succesfully." << std::endl;

	return 0;
}
