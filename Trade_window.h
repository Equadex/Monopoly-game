#ifndef TRADE_WINDOW_H
#define TRADE_WINDOW_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

#include "Question.h"
#include "Street.h"
#include "Text_field.h"
#include "Status_box.h"

class Trade_window : public Question{
public:
	Trade_window(int pos_x, int pos_y, Button **buttons, int n_buttons, char *title_in, char *message_in, ALLEGRO_FONT *button_text, ALLEGRO_FONT *title_font, ALLEGRO_BITMAP *image_in, Property **tomter_in, bool trade_prop_in);
	int get_n_buttons(){return n_buttons;}
	void set_n_buttons(int n_buttons_in){n_buttons = n_buttons_in;}
	void draw(bool &stage_1, bool &stage_2, bool &stage_3, int player_id_selected, Street **seller_streets, Street **buyer_streets, int n_seller_streets, int n_buyer_streets);
	void update(int seller_sum, int buyer_sum, Street **seller_streets, Street **buyer_streets, int n_seller_streets, int n_buyer_streets, Player *seller, Player *buyer);
	Status_box* get_status_buyer_boxes(){
		return(buyer_boxes);
	}
	Status_box* get_status_seller_boxes(){
		return(seller_boxes);
	}
	void set_status_seller_boxes(Status_box* box_in, int n_boxes){
		seller_boxes = box_in;
		n_seller_boxes = n_boxes;
	}
	void set_status_buyer_boxes(Status_box* box_in, int n_boxes){
		buyer_boxes = box_in;
		n_buyer_boxes = n_boxes;
	}
private:
	Property **tomter;
	ALLEGRO_FONT *button_text;
	ALLEGRO_FONT *title_font;
	Text_field **text_fields;
	int n_text_fields;
	int street_y_lines, street_x_lines;
	bool trade_prop;

	const int max_street_y_lines, max_street_x_lines;

	Status_box *buyer_boxes;
	Status_box *seller_boxes;
	int n_buyer_boxes;
	int n_seller_boxes;
};

#endif