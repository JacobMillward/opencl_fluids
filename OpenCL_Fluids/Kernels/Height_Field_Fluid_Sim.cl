#pragma OPENCL EXTENSION cl_khr_gl_sharing : enable

__kernel void ColumnSimStep(__global const float *u,
	__global float *u2,
	__global float *v,
	__constant int gridWidth,
	__constant float h,
	__constant float c2
	__constant float dt) {

	int i = get_global_id(0);
	int x = i % gridWidth;
	int y = i / gridWidth;

	float up = ( y - 1 < 0 ? 0 : u[i - gridWidth]);
	float down = (y + 1 > gridWidth ? 0 : u[i + gridWidth]);
	float left = (x - 1 < 0 ? 0 : u[i - 1]);
	float right = (x + 1 > gridWidth ? 0 : u[i + 1]);

	float f[gridWidth*gridWidth];
	f[i] = c2 *(up + down + left + right - 4 * u[i]) / (h * h);

	v[i] += f[i] * dt;

	u2[i] = u[i] + v[i] * dt;
}
