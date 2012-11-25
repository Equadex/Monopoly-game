#ifndef QUESTION_H
#define QUESTION_H

#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>

#include "Button.h"

class Question{
public:
	Question(int pos_x, int pos_y, Button *buttons[], std::string title, std::string message) : pos_x(pos_x), pos_y(pos_y), buttons(buttons), title(title), message(message) {}
private:
	ALLEGRO_BITMAP *image;
	int pos_x;
	int pos_y;
	Button **buttons;
	std::string title;
	std::string message;
};

#endif