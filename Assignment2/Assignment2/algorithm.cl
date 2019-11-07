
__kernel void algorithm_kernel(const int numMatrix1Cols, 
const int numMatrix1Rows,
__global const float* matrix1,
const int numMatrix2Cols, 
const int numMatrix2Rows,
__global const float* matrix2,
const int finalMatrixLength,
__global float* finalMatrix)
{
    int gid = (get_global_id(1) * finalMatrixLength) + get_global_id(0);

	float value = 0.0f;

	for(int i = 0; i < numMatrix1Cols; i++){
		int index1 = (get_global_id(1) * numMatrix1Cols) + i;
		int index2 = (i * numMatrix2Cols) + get_global_id(0);
		value += matrix1[index1] * matrix2[index2];
	}

	finalMatrix[gid] = value;
}
