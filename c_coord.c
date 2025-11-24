#include <math.h>

void c_coord (int n, float *x1, float *x2, float *y1, float *y2, float *z) {
	for (int i = 0; i < n; i++) {
		float xdiff = x2[i] - x1[i];
		float ydiff = y2[i] - y1[i];
		z[i] = sqrtf(xdiff * xdiff + ydiff * ydiff);
	}
}
