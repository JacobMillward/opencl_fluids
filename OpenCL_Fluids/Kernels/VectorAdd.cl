__kernel void VectorAdd(__global const int *a, __global const int *b, __global int *c) {
	//Get index of current elements
	int i = get_global_id(0);

	//Add elements togther
	c[i] = a[i] + b[i];
}