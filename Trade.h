#ifndef TRADE_H
#define TRADE_H

#include <allegro5\allegro.h>
//#include <allegro5\allegro_image.h>

#include "Trade_window.h"
#include "Button.h"


class Trade{
public:
	Trade(int pos_x, int pos_y, Property **tomter, ALLEGRO_FONT *button_text, ALLEGRO_FONT *title_font, ALLEGRO_BITMAP *trade_prop_image, ALLEGRO_BITMAP *button_image, ALLEGRO_BITMAP *box_image): player_draw_y_distance(35) {
		const int n_buttons_created = 11 + max_players;

		buttons = new Button*[n_buttons_created];
		buttons[0] = new Button(pos_x + 15, pos_y + 120, pos_x + 255, pos_y + 150, -3);
		buttons[1] = new Button(pos_x + 280, pos_y + 120, pos_x + 560, pos_y + 150, -2);
		buttons[2] = new Button(pos_x + 650, pos_y + 120, pos_x + 890, pos_y + 150, -1);
		buttons[3] = new Button(pos_x + 290, pos_y + 455, pos_x + 290 + al_get_bitmap_width(button_image), pos_y + 455 + al_get_bitmap_height(button_image), 0, "Trade", button_image);
		buttons[4] = new Button(pos_x + 530, pos_y + 455, pos_x + 530 + al_get_bitmap_width(button_image), pos_y + 455 + al_get_bitmap_height(button_image), 1, "Cancel", button_image);

		buttons[5] = new Button(pos_x + 270, pos_y + 405, pos_x + 270 + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 2, "1", box_image);
		buttons[6] = new Button(pos_x + 270 + (al_get_bitmap_width(box_image) * 2), pos_y + 405, pos_x + 270 + (al_get_bitmap_width(box_image) * 2) + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 3, "10", box_image);
		buttons[7] = new Button(pos_x + 270 + (al_get_bitmap_width(box_image) * 4), pos_y + 405, pos_x + 270 + (al_get_bitmap_width(box_image) * 4) + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 4, "100", box_image);

		buttons[8] = new Button(pos_x + 635, pos_y + 405, pos_x + 635 + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 5, "1", box_image);
		buttons[9] = new Button(pos_x + 635 + (al_get_bitmap_width(box_image) * 2), pos_y + 405, pos_x + 635 + (al_get_bitmap_width(box_image) * 2) + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 6, "10", box_image);
		buttons[10] = new Button(pos_x + 635 + (al_get_bitmap_width(box_image) * 4), pos_y + 405, pos_x + 635 + (al_get_bitmap_width(box_image) * 4) + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 7, "100", box_image);

		for(int i = 0; i < max_players; i++){
			char temp_char[100];
			sprintf(temp_char, "Player %i", i);
			buttons[11 + i] = new Button(pos_x + 40, pos_y + 180 + player_draw_y_distance * i, pos_x + 220, pos_y + 180 + player_draw_y_distance * (i + 1), 8 + i, temp_char);
		}


		window_proposition = new Trade_window(pos_x, pos_y, buttons, n_buttons_created, "Trade proposal", "", button_text, title_font, trade_prop_image, tomter);
 	}

	void draw(){
		if(window_proposition->get_active())
			window_proposition->draw();
		/*else if(window_approval->get_active())
			window_approval->draw();*/
	}
	void set_draw_proposition(bool value_in){
		window_proposition->set_active(value_in);
	}
	bool get_active(){
		return (window_proposition->get_active());
	}
	void pressed(int mouse_pos_x, int mouse_pos_y){

	}
private:
	Trade_window *window_proposition;
	Trade_window *window_approval;
	Button **buttons;
	int player_draw_y_distance;

	bool stage_1, stage_2, stage_3;
};

#endif