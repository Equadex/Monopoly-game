#ifndef TRADE_WINDOW_H
#define TRADE_WINDOW_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>

#include "Question.h"
#include "Street.h"
#include "Text_field.h"

class Trade_window : public Question{
public:
	Trade_window(int pos_x, int pos_y, Button **buttons, int n_buttons, char *title_in, char *message_in, ALLEGRO_FONT *button_text, ALLEGRO_FONT *title_font, ALLEGRO_BITMAP *image_in, Property **tomter_in) : tomter(tomter_in), button_text(button_text), title_font(title_font), n_text_fields(2), Question(pos_x, pos_y, buttons, n_buttons, title_in, message_in, image_in){
		text_fields = new Text_field*[2];
		text_fields[0] = new Text_field(460, pos_y + 405, "Sum: %i", button_text);
		text_fields[1] = new Text_field(805, pos_y + 405, "Sum: %i", button_text);
	}
	int get_n_buttons(){return n_buttons;}
	void set_n_buttons(int n_buttons_in){n_buttons = n_buttons_in;}
	void draw(bool &stage_1, bool &stage_2, bool &stage_3, int player_id_selected){
		if(active){
			al_draw_bitmap(image, pos_x, pos_y, 0);
			for(int i = 0; i < n_buttons; i++){
				buttons[i]->draw(button_text);
			}
			al_draw_text(title_font, al_map_rgb(0, 0, 0), pos_x + 430, pos_y + 40, ALLEGRO_ALIGN_CENTRE, title[0]);

			
			if(stage_1)
				al_draw_rectangle(pos_x + 15, pos_y + 115, pos_x + 260, pos_y + 440, al_map_rgb(0, 255, 0), 2);
			if(stage_2)
				al_draw_rectangle(pos_x + 280, pos_y + 115, pos_x + 570, pos_y + 440, al_map_rgb(0, 255, 0), 2);
			if(stage_3)
				al_draw_rectangle(pos_x + 610, pos_y + 115, pos_x + 895, pos_y + 440, al_map_rgb(0, 255, 0), 2);

			for(int i = 0; i < n_buttons; i++){
				if(buttons[i]->get_ID() == 11 + player_id_selected){
					al_draw_filled_rectangle(buttons[i]->get_pos_x_1() - 10, buttons[i]->get_pos_y_1() + 10 , buttons[i]->get_pos_x_1() , buttons[i]->get_pos_y_2() - 10, al_map_rgb(0, 255, 0));
				}
			}
			for(int i = 0; i < n_text_fields; i++){
				text_fields[i]->draw();
			}

		}
	}
	void update(int seller_sum, int buyer_sum){
		char temp[256];
		sprintf(temp, text_fields[0]->get_text_template(), seller_sum);
		text_fields[0]->set_text(temp);
		sprintf(temp, text_fields[1]->get_text_template(), buyer_sum);
		text_fields[1]->set_text(temp);
	}
private:
	Property **tomter;
	ALLEGRO_FONT *button_text;
	ALLEGRO_FONT *title_font;
	Text_field **text_fields;
	int n_text_fields;
};

#endif