#include "Main.h"
#include "Serial.h"
#include "OpenCL.h"

//	main() for HelloWorld example
int main(int argc, char** argv)
{
	std::cout << "Serially ran in " << Serial::SerialDemo() << " microseconds" << std::endl << std::endl;

	std::cout << "OpenCL ran in " << OpenCLDemo(CL_DEVICE_TYPE_CPU) << " microseconds" << std::endl << std::endl;

	std::cout << std::endl << std::endl;
	std::cout << "Executed program succesfully." << std::endl;

	return 0;
}
