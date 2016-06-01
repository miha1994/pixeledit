#include "object.h"
#include "fadma.h"
#include "picture_wind.h"

// фабрика объектов
Object *new_Object (string id) {
	SWITCH (id)
	CASE ("picture_wind") {
		picture_wind *o = new picture_wind;
		o->m_type = Type::Picture_wind;
		return o;
	}
	DEFAULT {
		error ("wrong type name '" + id + "'");
	}
}