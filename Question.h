#ifndef QUESTION_H
#define QUESTION_H

#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>

#include "Button.h"

class Question{
public:
	Question(int pos_x, int pos_y, Button *buttons[],int n_buttons, char* title, char* message, ALLEGRO_BITMAP *image_in, bool active = false) : pos_x(pos_x), pos_y(pos_y), buttons(buttons),n_buttons(n_buttons), title(title), message(message), image(image_in), active(active) {}
	void set_active(bool active_in){active = active_in;}
	bool get_active() const{return active;}
	
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text);
	int button_pressed(int mouse_pos_x, int mouse_pos_y);
private:
	bool active;
	ALLEGRO_BITMAP *image;
	int pos_x;
	int pos_y;
	int n_buttons;
	Button **buttons;
	char *title;
	char *message;
};

#endif