#pragma once
#include "object.h"
#include <string>
#include <vector>

using namespace std;

// спэйс так и будем называть спэйсом
struct Space {
	vector <Object *> m_objects_to_update;	// последовательность объектов, которые надо обновить
	vector <Object *> m_objects_to_render;	// последовательность объектов, которые надо отрисовать
	Space (string name);	// конструктор
	~Space () {				// деструктор
		forstl_p (p, m_objects_to_update) {
			delete p;
		}
	}
};