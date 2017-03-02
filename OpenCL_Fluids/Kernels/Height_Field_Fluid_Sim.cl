__kernel void ColumnSimStep(__global const float3 *u,
	__global float3 *u2,
	__global float *v,
	const int gridWidth,
	const float h,
	const float c2,
	const float maxSlope,
	const float dt) {

	int i = get_global_id(0);
	int x = i % gridWidth;
	int y = i / gridWidth;

	float up = ( y - 1 < 0 ? 0 : u[i - gridWidth].y);
	float down = (y + 1 >= gridWidth ? 0 : u[i + gridWidth].y);
	float left = (x - 1 < 0 ? 0 : u[i - 1].y);
	float right = (x + 1 >= gridWidth ? 0 : u[i + 1].y);

	float f = c2 *(up + down + left + right - 4 * u[i].y) / (h * h);

	v[i] += f * dt;

	u2[i].y = u[i].y + v[i] * dt;

	float offset = ((up + down + left + right) / 4) - u[i].y;
	float maxOffset = maxSlope * h;
	if (offset > maxOffset) u2[i].y += offset - maxOffset;
	if (offset < -maxOffset) u2[i].y += offset + maxOffset;
}
