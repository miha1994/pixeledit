#pragma once
#include "math_3d.h"

#define D_UP	0
#define D_RIGHT	1
#define D_DOWN	2
#define D_LEFT	3

extern v2i g_n_to_dir[4];
int g_dir_to_n (v2i dir);
int g_dir_to_n (v2f dir);