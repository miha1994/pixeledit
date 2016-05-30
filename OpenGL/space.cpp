#include "space.h"
#include "read_write.h"
#include "fadma.h"
#include <map>
#include "glob.h"

Space::Space (string name) {
	string tmp_str = prefix_folder + "res/inf/" + name + "_space.txt";
	freopen (tmp_str.c_str (), "r", stdin);
	read_string (); // "{"
	string chapter;

	map <string, Object *> objects;
	for (chapter = read_string (); chapter != "}"; chapter = read_string ()) {
		SWITCH (chapter)
		CASE ("UPDATE_SEQUENCE") {
			read_string (); // "{"
			string str;
			while ((str = read_string ()) != "}") {
				if (objects.find (str) == objects.end ()) {
					objects[str] = new_Object (str);
				}
				m_objects_to_update.push_back (objects[str]);
			}
		}
		CASE ("RENDER_SEQUENCE") {
			read_string (); // "{"
			string str;
			while ((str = read_string ()) != "}") {
				if (objects.find (str) == objects.end ()) {
					objects[str] = new_Object (str);
				}
				m_objects_to_render.push_back (objects[str]);
			}
		}
		CASE ("DEF") {
			read_string (); // "{"
			string str;
			while ((str = read_string ()) != "}") {
				if (objects.find (str) == objects.end ()) {
					objects[str] = new_Object (str);
				}
				read_string (); // "{"
				objects[str]->load ();
			}
		}
		DEFAULT {
			error ("symbol '" + chapter + "' in file   res/inf/" + name + "_space.txt    expected 'UPDATE_SEQUENCE' or 'DEF' or ...");
		}
	}
	objects.clear ();
}