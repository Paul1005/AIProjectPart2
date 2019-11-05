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

//  Constants
const int ARRAY_SIZE = 10;
int array_size = 0;

//	main() for HelloWorld example
int main(int argc, char** argv)
{
	array_size = ARRAY_SIZE;
	if (argc > 1)
		array_size = atoi(argv[1]);

	cl_context context = 0;
	cl_command_queue commandQueue = 0;
	cl_program program = 0;
	cl_device_id device = 0;
	cl_kernel kernel = 0;
	cl_mem memObjects[3] = { 0, 0, 0 };
	cl_int errNum;

	// Create an OpenCL context on first available platform
	context = CreateContext();
	if (context == NULL)
	{
		std::cerr << "Failed to create OpenCL context." << std::endl;
		return 1;
	}

	// Create a command-queue on the first device available
	// on the created context
	commandQueue = CreateCommandQueue(context, &device);
	if (commandQueue == NULL)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

	// Create OpenCL program from HelloWorld.cl kernel source
	program = CreateProgram(context, device, "HelloWorld.cl");
	if (program == NULL)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

	// Create OpenCL kernel
	kernel = clCreateKernel(program, "hello_kernel", NULL);
	if (kernel == NULL)
	{
		std::cerr << "Failed to create kernel" << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}

	// Create memory objects that will be used as arguments to
	// kernel.  First create host memory arrays that will be
	// used to store the arguments to the kernel
	float* result = new float[array_size];
	float* a = new float[array_size];
	float* b = new float[array_size];
	for (int i = 0; i < array_size; i++)
	{
		a[i] = (float)i;
		b[i] = (float)(i * 2);
	}

	CTiming timer;
	int seconds, useconds;
	timer.Start();
	for (int i = 0; i < array_size; i++)
	{
		result[i] = a[i] + b[i];
	}
	timer.End();
	if (timer.Diff(seconds, useconds))
		std::cerr << "Warning: timer returned negative difference!" << std::endl;
	std::cout << "Serially ran in " << seconds << "." << useconds << " seconds" << std::endl << std::endl;

	if (!CreateMemObjects(context, memObjects, a, b))
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] b;
		delete[] a;
		delete[] result;
		return 1;
	}

	// Set the kernel arguments (result, a, b)
	errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
	errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
	errNum |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[2]);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error setting kernel arguments." << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] b;
		delete[] a;
		delete[] result;
		return 1;
	}

	size_t globalWorkSize[1] = { array_size };
	size_t localWorkSize[1] = { 1 };

	timer.Start();

	// Queue the kernel up for execution across the array
	errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 1, NULL,
		globalWorkSize, localWorkSize,
		0, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error queuing kernel for execution." << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] b;
		delete[] a;
		delete[] result;
		return 1;
	}

	// Read the output buffer back to the Host
	errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE,
		0, array_size * sizeof(float), result,
		0, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		std::cerr << "Error reading result buffer." << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] b;
		delete[] a;
		delete[] result;
		return 1;
	}

	timer.End();
	if (timer.Diff(seconds, useconds))
		std::cerr << "Warning: timer returned negative difference!" << std::endl;
	std::cout << "OpenCL ran in " << seconds << "." << useconds << " seconds" << std::endl << std::endl;

	// Output (some of) the result buffer
	for (int i = 0; i < ((array_size > 100) ? 100 : array_size); i++)
	{
		std::cout << result[i] << " ";
	}
	std::cout << std::endl << std::endl;
	std::cout << "Executed program succesfully." << std::endl;
	Cleanup(context, commandQueue, program, kernel, memObjects);
	delete[] b;
	delete[] a;
	delete[] result;

	return 0;
}