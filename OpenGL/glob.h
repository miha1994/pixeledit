#pragma once
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "fadma.h"

extern string prefix_folder;
extern string prefix_path;
extern GLuint gSampler;

#define D_(v)	(v).x][(v).y
#define Texture_name(name)	(prefix_folder + "res/tex/" + string (name) + ".png")

#define CLR		sf::Color

CLR operator * (CLR c, float f);
CLR operator ^ (CLR c, CLR s);