#ifndef TRADE_WINDOW_H
#define TRADE_WINDOW_H

#include <allegro5\allegro.h>
#include <allegro5\allegro_font.h>

#include "Question.h"
#include "Street.h"

class Trade_window : public Question{
public:
	Trade_window(int pos_x, int pos_y, Button **buttons, int n_buttons, char *title_in, char *message_in, ALLEGRO_FONT *button_text, ALLEGRO_FONT *title_font, ALLEGRO_BITMAP *image_in, Property **tomter_in) : tomter(tomter_in), button_text(button_text), title_font(title_font), Question(pos_x, pos_y, buttons, n_buttons, title_in, message_in, image_in){

	}
	void draw(){
		if(active){
			al_draw_bitmap(image, pos_x, pos_y, 0);
			for(int i = 0; i < n_buttons; i++){
				buttons[i]->draw(button_text);
			}
			al_draw_text(title_font, al_map_rgb(0, 0, 0), pos_x + 430, pos_y + 40, ALLEGRO_ALIGN_CENTRE, title[0]);
		}
	}
private:
	Property **tomter;
	ALLEGRO_FONT *button_text;
	ALLEGRO_FONT *title_font;
};

#endif