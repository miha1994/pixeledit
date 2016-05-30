#pragma once
#include "object.h"
#include "Renderer.h"
#include "space.h"
#include <vector>
#include <map>

using namespace std;

class Core {
public:
	void init ();
	void update (float dt);
	void render ();

	Renderer m_renderer;
	State m_current_state;
	map <string, Space *> m_spaces;
	vector <Space *> m_current_spaces;

	Core () {}
	~Core () {
		forstl (p, m_spaces) {
			delete p->second;
		}
	}
};

extern Core core;