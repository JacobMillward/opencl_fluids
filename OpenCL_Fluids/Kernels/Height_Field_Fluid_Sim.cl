#pragma OPENCL EXTENSION cl_khr_gl_sharing : enable

__kernel void ColumnSimStep(__global const float *u,
	__global float *u2,
	__global float *v,
	const int gridWidth,
	const float h,
	const float c2,
	const float dt) {

	int i = get_global_id(0);
	int x = i % gridWidth;
	int y = i / gridWidth;

	printf("CL Read Buffer Value (%i): %f\n", i, ++u2[i]);
	
	/*float up = ( y - 1 < 0 ? 0 : u[i - gridWidth]);
	float down = (y + 1 > gridWidth ? 0 : u[i + gridWidth]);
	float left = (x - 1 < 0 ? 0 : u[i - 1]);
	float right = (x + 1 > gridWidth ? 0 : u[i + 1]);

	float f = c2 *(up + down + left + right - 4 * u[i]) / (h * h);

	v[i] += f * dt;

	u2[i] = u[i] + v[i] * dt;*/
}
