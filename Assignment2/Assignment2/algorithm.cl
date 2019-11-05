
__kernel void algorithm_kernel(__global const float *a,
						__global const float *b,
						__global float *result)
{
	int const max = 3;
	int const min = 2;
	int const height = min;
	int const width = min;
	float** finalMatrix = new float*[height];

	for (int h = 0; h < height; h++)
	{
		finalMatrix[h] = new float[width];
	}

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
