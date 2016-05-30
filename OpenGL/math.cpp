#include "math_3d.h"

// Matrix4f
void Matrix4f::operator *=(Matrix4f w) {
	Matrix4f res;
    FOR (i, 4) {
        FOR (j, 4) {
            res.m[i][j] = 0;
        }
    }
    FOR (i, 4) {
        FOR (j, 4) {
            FOR (z, 4) {
                res.m[i][j] += me.m[i][z] * w.m[z][j];
            }
        }
    }
    FOR (i, 4) {
        FOR (j, 4) {
            me.m[i][j] = res.m[i][j];
        }
    }
}