#pragma once
#include "object.h"
#include <string>
#include <vector>

using namespace std;

struct Space {
	vector <Object *> m_objects_to_update;
	vector <Object *> m_objects_to_render;
	Space (string name);
	~Space () {
		forstl_p (p, m_objects_to_update) {
			delete p;
		}
	}
};