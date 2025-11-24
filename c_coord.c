#include <math.h>

void c_coord (int n, float *x1, float *x2, float *y1, float *y2, float *z) {
	for (int i = 0; i < n; i++) {
		z[i] = sqrtf(pow(x2[i] - x1[i], 2) + pow(y2[i] - y1[i], 2));
	}
}
