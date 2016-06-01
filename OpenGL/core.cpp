#include "core.h"
#include "fadma.h"
#include "input.h"

Core core;

void Core::init () {
	m_current_state = State_0;
	m_spaces["main"] = new Space ("main");	// здесь парсится файл main_space.txt и создается основной спэйс
	m_current_spaces.push_back (m_spaces["main"]);	// он назначается активным

	m_renderer.init ();
}

void Core::update (float dt) {
	in.upd ();
	forstl_p (q, m_current_spaces) {
		forstl_p (p, q->m_objects_to_update) {
			p->update (m_current_state, dt);
		}
	}
}

void Core::render () {
	forstl_p (q, m_current_spaces) {
		forstl_p (p, q->m_objects_to_render) {
			p->render (m_current_state);
		}
	}
}