#pragma once


#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>
#endif

long OpenCLDemo(cl_device_type type);

bool CheckOpenCLError(cl_int errNum, const char* errMsg);
cl_context CreateContext();
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id* device);
cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName);
bool CreateMemObjects(cl_context context, cl_mem memObjects[3], float* a, float* b);
void Cleanup(cl_context context, cl_command_queue commandQueue, cl_program program, cl_kernel kernel, cl_mem memObjects[3]);
