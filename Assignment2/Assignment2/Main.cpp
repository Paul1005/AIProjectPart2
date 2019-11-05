#include "Serial.h"
#include "OpenCL.h"
#include <iostream>

int main(int argc, char** argv)
{
	try
	{
		std::cout << "Running algorithm serially..." << std::endl;
		std::cout << "Serially ran in " << Serial::SerialDemo() << " microseconds" << std::endl << std::endl;
	}
	catch (std::runtime_error ex)
	{
		std::cerr << "Serial: " << ex.what() << std::endl;
	}

	try
	{
		std::cout << "Running algorithm on OpenCL CPU..." << std::endl;
		std::cout << "OpenCL ran in " << OpenCLDemo(CL_DEVICE_TYPE_CPU) << " microseconds" << std::endl << std::endl;
	}
	catch (std::runtime_error ex)
	{
		std::cerr << "OpenCL CPU: " << ex.what() << std::endl;
	}

	try
	{
		std::cout << "Running algorithm on OpenCL GPU..." << std::endl;
		std::cout << "OpenCL ran in " << OpenCLDemo(CL_DEVICE_TYPE_GPU) << " microseconds" << std::endl << std::endl;
	}
	catch (std::runtime_error ex)
	{
		std::cerr << "OpenCL GPU: " << ex.what() << std::endl;
	}

	try
	{
		std::cout << "Running algorithm on OpenCL CPU+GPU..." << std::endl;
		std::cout << "OpenCL ran in " << OpenCLDemo(CL_DEVICE_TYPE_CPU|CL_DEVICE_TYPE_GPU) << " microseconds" << std::endl << std::endl;
	}
	catch (std::runtime_error ex)
	{
		std::cerr << "OpenCL CPU+GPU: " << ex.what() << std::endl;
	}

	std::cout << std::endl << std::endl;
	std::cout << "Executed program succesfully." << std::endl;

	return 0;
}
