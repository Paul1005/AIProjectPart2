/////////////////////////////////////////////////////////////////////////////////
//
// Sample OpenCL application
// (c) 2015 Borna Noureddin
// British Columbia Institute of Technology
// Adapted from: OpenCL(R) Programming Guide
//      Authors: Aaftab Munshi, Benedict Gaster, Timothy Mattson, James Fung, Dan Ginsburg
//
/////////////////////////////////////////////////////////////////////////////////


#include "OpenCL.h"
#include <chrono>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>    // std::min
#include <cmath>

//  Create an OpenCL context on the first available platform using
//  either a GPU or CPU depending on what is available.
cl_context CreateContext(cl_device_type type)
{
	// First, select an OpenCL platform to run on.  For this example, we
	// simply choose the first available platform.  Normally, you would
	// query for all available platforms and select the most appropriate one.
	cl_platform_id platforms[2] = { nullptr };
	cl_uint numPlatforms;
	cl_int errNum = clGetPlatformIDs(2, platforms, &numPlatforms);
	if (errNum != CL_SUCCESS)
		throw std::runtime_error("Failed to find any OpenCL platforms.");

	std::cout << std::endl << numPlatforms << " platforms in total" << std::endl;

	cl_context context = NULL;

	char pname[1024];

	// iterate over all platforms and find a valid device
	for (cl_platform_id p : platforms)
	{
		if (p == nullptr) break;

		// Get information about the platform
		size_t retsize;
		errNum = clGetPlatformInfo(p, CL_PLATFORM_NAME, sizeof(pname), (void*)pname, &retsize);
		if (errNum != CL_SUCCESS)
		{
			std::cerr << "Could not get platform info" << std::endl;
			continue;
		}

		// Next, create an OpenCL context on the platform
		cl_context_properties contextProperties[] =
		{
			CL_CONTEXT_PLATFORM,
			(cl_context_properties)p,
			0
		};
		//std::cout << std::endl << "Selected platform <" << pname << ">" << std::endl;
		context = clCreateContextFromType(contextProperties, type, NULL, NULL, &errNum);
		if (errNum != CL_SUCCESS)
			throw std::runtime_error("Failed to create an OpenCL context.");
		else {
			break;
		}
	}

	std::cout << std::endl << "Selected platform <" << pname << ">" << std::endl;

	return context;
}

//  Create a command queue on the first device available on the context
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id* device)
{
	// Get number of devices
	cl_int numDevices;
	size_t retSize;
	cl_int errNum = clGetContextInfo(context, CL_CONTEXT_NUM_DEVICES, sizeof(numDevices), (void*)& numDevices, &retSize);
	if (errNum != CL_SUCCESS)
		throw std::runtime_error("Could not get context info!");

	//std::cout << std::endl << "There are " << numDevices << " devices." << std::endl;


	// Get list of devices
	cl_device_id* deviceList;
	deviceList = (cl_device_id*)malloc(numDevices * sizeof(cl_device_id));
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, numDevices * sizeof(cl_device_id), (void*)deviceList, &retSize);
	switch (errNum)
	{
		//std::cerr << " size = " << numDevices * sizeof(cl_device_id) << ";" << retSize << std::endl; // Additional info, ignoring for now.
	case CL_INVALID_CONTEXT:
		throw std::runtime_error("Could not get context info! ERROR code (CL_INVALID_CONTEXT)");
		break;
	case CL_INVALID_VALUE:
		throw std::runtime_error("Could not get context info! ERROR code (CL_INVALID_VALUE)");
		break;
	case CL_OUT_OF_RESOURCES:
		throw std::runtime_error("Could not get context info! ERROR code (CL_OUT_OF_RESOURCES)");
		break;
	case CL_OUT_OF_HOST_MEMORY:
		throw std::runtime_error("Could not get context info! ERROR code (CL_OUT_OF_HOST_MEMORY)");
		break;
	case CL_SUCCESS:
		break;
	default:
		break;
	}

	// Get device information for each device
	cl_device_type devType;
	std::cout << std::endl << "Device list:" << std::endl;
	for (int i = 0; i < numDevices; i++)
	{

		std::cout << "   " << deviceList[i] << ": ";

		// device type
		errNum = clGetDeviceInfo(deviceList[i], CL_DEVICE_TYPE, sizeof(cl_device_type), (void*)& devType, &retSize);
		if (errNum != CL_SUCCESS)
		{
			free(deviceList);
			throw std::runtime_error("ERROR getting device info!");
		}


		std::cout << " type " << devType << ":";
		if (devType & CL_DEVICE_TYPE_CPU)
			std::cout << " CPU";
		if (devType & CL_DEVICE_TYPE_GPU)
			std::cout << " GPU";
		if (devType & CL_DEVICE_TYPE_ACCELERATOR)
			std::cout << " accelerator";
		if (devType & CL_DEVICE_TYPE_DEFAULT)
			std::cout << " default";


		// device name
		char devName[1024];
		errNum = clGetDeviceInfo(deviceList[i], CL_DEVICE_NAME, 1024, (void*)devName, &retSize);
		if (errNum != CL_SUCCESS)
		{
			free(deviceList);
			throw std::runtime_error("ERROR getting device name!");
		}
		//std::cout << " name=<" << devName << ">" << std::endl;

	}
	std::cout << std::endl;


	// In this example, we just choose the first available device.  In a
	// real program, you would likely use all available devices or choose
	// the highest performance device based on OpenCL device queries
	cl_command_queue commandQueue = clCreateCommandQueue(context, deviceList[0], 0, NULL);
	if (commandQueue == NULL)
	{
		free(deviceList);
		std::cerr << "Failed to create commandQueue for device 0";
		return NULL;
	}

	*device = deviceList[0];

	free(deviceList);

	return commandQueue;
}



//  Create an OpenCL program from the kernel source file
cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName)
{
	cl_int errNum;
	cl_program program;

	std::ifstream kernelFile(fileName, std::ios::in);
	if (!kernelFile.is_open())
	{
		std::cerr << "Failed to open file for reading: " << fileName << std::endl;
		return NULL;
	}

	std::ostringstream oss;
	oss << kernelFile.rdbuf();

	std::string srcStdStr = oss.str();
	const char* srcStr = srcStdStr.c_str();
	program = clCreateProgramWithSource(context, 1,
		(const char**)& srcStr,
		NULL, NULL);
	if (program == NULL)
	{
		std::cerr << "Failed to create CL program from source." << std::endl;
		return NULL;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		// Determine the reason for the error
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			sizeof(buildLog), buildLog, NULL);

		std::cerr << "Error in kernel: " << std::endl;
		std::cerr << buildLog;
		clReleaseProgram(program);
		return NULL;
	}

	return program;
}



//  Create memory objects used as the arguments to the kernel
//  The kernel takes three arguments: result (output), a (input), and b (input)
bool CreateMemObjects(cl_context context, cl_mem memObjects[3], float* matrix1, float* matrix2, float* finalMatrix)
{
	int matrix1Size = sizeof(matrix1);
	int matrix2Size = sizeof(matrix2);
	int finalMatrixSize = sizeof(finalMatrix);
	memObjects[0] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(float) * matrix1Size, matrix1, NULL);
	memObjects[1] = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
		sizeof(float) * matrix2Size, matrix2, NULL);
	memObjects[2] = clCreateBuffer(context, CL_MEM_READ_WRITE,
		sizeof(float) * finalMatrixSize, NULL, NULL);

	if (memObjects[0] == NULL || memObjects[1] == NULL || memObjects[2] == NULL)
	{
		std::cerr << "Error creating memory objects." << std::endl;
		return false;
	}

	return true;
}



//  Cleanup any created OpenCL resources
void Cleanup(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem memObjects[3])
{
	for (int i = 0; i < 3; i++)
	{
		if (memObjects[i] != 0)
			clReleaseMemObject(memObjects[i]);
	}
	if (commandQueue != 0)
		clReleaseCommandQueue(commandQueue);

	if (kernel != 0)
		clReleaseKernel(kernel);

	if (program != 0)
		clReleaseProgram(program);

	if (context != 0)
		clReleaseContext(context);

}



long OpenCLDemo(cl_device_type type)
{
	cl_context context = 0;
	cl_command_queue commandQueue = 0;
	cl_program program = 0;
	cl_device_id device = 0;
	cl_kernel kernel = 0;
	cl_mem memObjects[3] = { 0, 0, 0 };
	cl_int errNum;

	// Create an OpenCL context on first available GPU
	context = CreateContext(type);
	if (context == NULL)
	{
		throw std::runtime_error("Failed to create OpenCL context.");
	}

	// Create a command-queue on the first device available
	// on the created context
	commandQueue = CreateCommandQueue(context, &device);
	if (commandQueue == NULL)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		throw std::runtime_error("Failed to create OpenCL command queue.");
	}

	// Create OpenCL program from algorithm.cl kernel source
	program = CreateProgram(context, device, "algorithm.cl");
	if (program == NULL)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		throw std::runtime_error("Failed to create OpenCL program.");
	}

	// Create OpenCL kernel
	kernel = clCreateKernel(program, "algorithm_kernel", NULL);
	if (kernel == NULL)
	{
		std::cerr << "Failed to create kernel" << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		throw std::runtime_error("Failed to create OpenCL kernel.");
	}

	const int numMatrix1Cols = 3;
	const int numMatrix1Rows = 2;
	const int numMatrix2Cols = 2;
	const int numMatrix2Rows = 3;

	// Create memory objects that will be used as arguments to
	// kernel.  First create host memory arrays that will be
	// used to store the arguments to the kernel
	const int matrix1Size = numMatrix1Cols * numMatrix1Rows;
	float* matrix1 = new float[matrix1Size];
	float value = 1.0f;
	for (int i = 0; i < matrix1Size; i++) {
		matrix1[i] = value;
		value++;
	}

	const int matrix2Size = numMatrix2Cols * numMatrix2Rows;
	float* matrix2 = new float[matrix2Size];
	for (int i = 0; i < matrix2Size; i++) {
		matrix2[i] = value;
		value++;
	}

	const int numFinalMatrixRows = std::min(numMatrix1Rows, numMatrix2Rows);
	const int numFinalMatrixCols = std::min(numMatrix1Cols, numMatrix2Cols);
	const int finalMatrixSize = numFinalMatrixRows * numFinalMatrixCols;
	const int finalMatrixLength = std::sqrt(finalMatrixSize);

	float* finalMatrix = new float[finalMatrixSize];
	for (int i = 0; i < finalMatrixSize; i++) {
		finalMatrix[i] = 0.0f;
	}

	if (!CreateMemObjects(context, memObjects, matrix1, matrix2, finalMatrix))
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Failed to create OpenCL memory objects.");
	}

	errNum = clSetKernelArg(kernel, 0, sizeof(cl_int), &numMatrix1Cols);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	errNum = clSetKernelArg(kernel, 1, sizeof(cl_int), &numMatrix1Rows);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	errNum = clSetKernelArg(kernel, 2, sizeof(cl_mem), &memObjects[0]);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	errNum = clSetKernelArg(kernel, 3, sizeof(cl_int), &numMatrix2Cols);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	errNum = clSetKernelArg(kernel, 4, sizeof(cl_int), &numMatrix2Rows);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	errNum = clSetKernelArg(kernel, 5, sizeof(cl_mem), &memObjects[1]);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	errNum = clSetKernelArg(kernel, 6, sizeof(cl_int), &finalMatrixLength);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	errNum = clSetKernelArg(kernel, 7, sizeof(cl_mem), &memObjects[2]);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error setting kernel arguments.");
	}

	size_t globalWorkSize[2] = { static_cast<size_t>(numFinalMatrixCols), static_cast<size_t>(numFinalMatrixRows) };
	size_t localWorkSize[2] = { 2,2 };

	auto start = std::chrono::steady_clock::now();

	// Queue the kernel up for execution across the array
	errNum = clEnqueueNDRangeKernel(commandQueue, kernel, 2, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		std::cout << "errNum: " << errNum << std::endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error queuing kernel for execution.");
	}

	// Read the output buffer back to the Host
	errNum = clEnqueueReadBuffer(commandQueue, memObjects[2], CL_TRUE, 0, sizeof(float) * finalMatrixSize, finalMatrix, 0, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		Cleanup(context, commandQueue, program, kernel, memObjects);
		delete[] matrix1;
		delete[] matrix2;
		delete[] finalMatrix;
		throw std::runtime_error("Error reading result buffer.");
	}

	auto finish = std::chrono::steady_clock::now();

	// Output (some of) the result buffer
	for (int i = 0; i < finalMatrixSize; i++) {
		std::cout << finalMatrix[i] << ' ';
		if (i == (finalMatrixSize / 2) - 1) {
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;

	Cleanup(context, commandQueue, program, kernel, memObjects);
	delete[] matrix1;
	delete[] matrix2;
	delete[] finalMatrix;

	return std::chrono::duration_cast<std::chrono::nanoseconds>(finish - start).count();
}
