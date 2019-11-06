
__kernel void algorithm_kernel(__global float** finalMatrix,
						__global const float** matrix1,
						__global const float** matrix2)
{
	int const height = 2;
	int const width = 2;
	int const max = 3;

	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			float value = 0.0f;
			for (int k = 0; k < max; k++) {
				value += (matrix1[i][k] * matrix2[k][j]);
			}
			finalMatrix[i][j] = value;
		}
	}

	return finalMatrix;
}
