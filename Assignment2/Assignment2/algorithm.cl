
__kernel void algorithm_kernel(__global const float* matrix1row1, 
__global const float* matrix1row2, 
__global const float* matrix2col1, 
__global const float* matrix2col2, 
__global float* finalMatrix)
{
    int gid = get_global_id(0);

	finalMatrix[0] += (matrix1row1[gid]*matrix2col1[gid]);

	finalMatrix[1] += (matrix1row1[gid]*matrix2col2[gid]);
	
	finalMatrix[2] += (matrix1row2[gid]*matrix2col1[gid]);

	finalMatrix[3] += (matrix1row2[gid]*matrix2col2[gid]);
}
