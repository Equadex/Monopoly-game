#ifndef TRADE_WINDOW_H
#define TRADE_WINDOW_H

#include "Question.h"
#include "Text_field.h"
#include "Street.h"

class Trade_window : public Question{
public:
	Trade_window(int pos_x, int pos_y, Button **buttons, int n_buttons, char *title_in, char *message_in, ALLEGRO_BITMAP *image_in, Text_field **text_fields_player_list_in, Property **tomter_in) : text_fields_player_list(text_fields_player_list_in), tomter(tomter_in), Question(pos_x, pos_y, buttons, n_buttons, title_in, message_in, image_in){

	}
private:
	Text_field **text_fields_player_list;
	Property **tomter;
};

#endif