#ifndef QUESTION_H
#define QUESTION_H


#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

#include "Button.h"

//class Property; //All foward-declearations in this class should not be here!
//class Street;
//class Player;
//class Status_box;

const int max_question_length = 80;
const int max_question_lines = 10;
const int max_question_lines_title = 3;

class Question{
public:
	Question(int pos_x, int pos_y, Button *buttons[],int n_buttons, char* title_in, char* message_in, ALLEGRO_BITMAP *image_in, bool active = false);
	Question(int pos_x, int pos_y, ALLEGRO_BITMAP *image_in, bool active = false): pos_x(pos_x), pos_y(pos_y), image(image_in), active(active) {}
	~Question();
	void set_active(bool active_in){active = active_in;}
	bool get_active() const{return active;}
	int get_pos_x() const {return pos_x;}
	int get_pos_y() const {return pos_y;}
	int get_n_buttons() const {return n_buttons;}
	char** get_message(int &n_message_out) const {n_message_out = n_message; return message;}
	
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text);
	void set_text(char *title_in, char* message_in);
	int button_pressed(int mouse_pos_x, int mouse_pos_y);
	void set_orignal_message();
	char** get_original_message(int &n_message_out) const {n_message_out = n_orignal_message; return orignal_message;}

protected:
	bool active;
	ALLEGRO_BITMAP *image;
	int pos_x;
	int pos_y;
	int n_buttons;
	Button **buttons;
	char **title;
	int n_title, n_message, n_orignal_message;
	char **message;
	char **orignal_message;
};

#endif