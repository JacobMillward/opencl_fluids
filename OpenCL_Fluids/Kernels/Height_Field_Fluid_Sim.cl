__kernel void ColumnSimStep(__global const float3 *u,
	__global float3 *u2,
	__global float *v,
	const int gridWidth,
	const float h,
	const float c2,
	const float dt) {

	int i = get_global_id(0);
	int x = i % gridWidth;
	int y = i / gridWidth;
	printf("%f\n", u[i].x);
	float3 up = ( y - 1 < 0 ? 0 : u[i - gridWidth]);
	float3 down = (y + 1 > gridWidth ? 0 : u[i + gridWidth]);
	float3 left = (x - 1 < 0 ? 0 : u[i - 1]);
	float3 right = (x + 1 > gridWidth ? 0 : u[i + 1]);

	float f = c2 *(up.y + down.y + left.y + right.y - 4 * u[i].y) / (h * h);

	v[i] += f * dt;

	u2[i].y = u[i].y + v[i] * dt;
}
