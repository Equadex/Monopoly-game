#ifndef QUESTION_H
#define QUESTION_H


#include <string>

#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_font.h>

#include "Button.h"
#include "Text_field.h"

const int max_question_length = 90;
const int max_question_lines = 10;
const int max_question_lines_title = 3;

class Question{
public:
	Question(int pos_x, int pos_y, Button *buttons[],int n_buttons, char* title_in, char* message_in, ALLEGRO_BITMAP *image_in, bool active = false);
	~Question();
	void set_active(bool active_in){active = active_in;}
	bool get_active() const{return active;}
	
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text);
	int button_pressed(int mouse_pos_x, int mouse_pos_y);

protected:
	bool active;
	ALLEGRO_BITMAP *image;
	int pos_x;
	int pos_y;
	int n_buttons;
	Button **buttons;
	char **title;
	int n_title, n_message;
	char **message;
};

class Auction_window : public Question{
public:
	Auction_window(int pos_x, int pos_y, Button *buttons[],int n_buttons, char* title_in, char* message_in, ALLEGRO_BITMAP *image_in, Text_field** text_fields_in, int ant_text_fields_in, bool active = false) : Question(pos_x, pos_y, buttons, n_buttons, title_in, message_in, image_in, active), text_fields(text_fields_in), ant_text_fields(ant_text_fields_in){}
	void draw(ALLEGRO_FONT* Title, ALLEGRO_FONT* Text);

	void update(char* name, int c_bid, int p_bid, int c_player);
protected:
	Text_field **text_fields;
	int ant_text_fields;
private:
};

#endif