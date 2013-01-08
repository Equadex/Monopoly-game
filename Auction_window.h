#ifndef AUCTION_WINDOW_H
#define AUCTION_WINDOW_H

#include "Question.h"
#include "Text_field.h" //Used for drawing

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