#ifndef TRADE_H
#define TRADE_H

#include <allegro5\allegro.h>
//#include <allegro5\allegro_image.h>

#include "Trade_window.h"
#include "Button.h"
#include "Player.h"


class Trade{
public:
	Trade(int pos_x, int pos_y, Property **tomter, ALLEGRO_FONT *button_text, ALLEGRO_FONT *title_font, ALLEGRO_BITMAP *trade_prop_image, ALLEGRO_BITMAP *button_image, ALLEGRO_BITMAP *box_image): stage_1(false), stage_2(false), stage_3(false), player_draw_y_distance(35), seller(0), buyer(0), sum_seller(0), sum_buyer(0) {
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
			buttons[11 + i] = new Button(pos_x + 40, pos_y + 180 + player_draw_y_distance * i, pos_x + 220, pos_y + 180 + player_draw_y_distance * (i + 1), 11 + i, temp_char);
		}


		window_proposition = new Trade_window(pos_x, pos_y, buttons, n_buttons_created, "Trade proposal", "", button_text, title_font, trade_prop_image, tomter);
 	}

	void draw(){
		if(window_proposition->get_active())
			if(seller != NULL)
				window_proposition->draw(stage_1, stage_2, stage_3, seller->get_id());
			else
				window_proposition->draw(stage_1, stage_2, stage_3, -1);
		/*else if(window_approval->get_active())
			window_approval->draw();*/
	}
	void set_draw_proposition(bool value_in, int current_players){
		window_proposition->set_n_buttons(window_proposition->get_n_buttons() - max_players + current_players);
		window_proposition->set_active(value_in);
		window_proposition->update(sum_seller, sum_buyer);
	}
	bool get_active(){
		return (window_proposition->get_active());
	}
	void set_buyer(Player *buyer_in){
		buyer = buyer_in;
	}
	void pressed(int mouse_pos_x, int mouse_pos_y, Player **players, bool rightclick = false){
		int button_id = window_proposition->button_pressed(mouse_pos_x, mouse_pos_y);

		if(button_id == -3){
			stage_1 = true;
			stage_2 = false;
			stage_3 = false;
		}
		else if(button_id == -2){
			if(stage_1){
				stage_1 = false;
				stage_2 = true;
				stage_3 = false;
			}
		}
		else if(button_id == -1){
			if(stage_2){
				stage_1 = false;
				stage_2 = false;
				stage_3 = true;
			}
		}
		else if(button_id == 0){
			if(stage_3){
				;
			}
		}
		else if(button_id == 2 && stage_2){
			if(!rightclick)
				sum_seller++;
			else
				sum_seller--;
		}
		else if(button_id == 3 && stage_2){
			if(!rightclick)
				sum_seller += 10;
			else
				sum_seller -= 10;
		}
		else if(button_id == 4 && stage_2){
			if(!rightclick)
				sum_seller += 100;
			else
				sum_seller -= 100;
		}
		else if(button_id == 5 && stage_3){
			if(!rightclick)
				sum_buyer++;
			else
				sum_buyer--;
		}
		else if(button_id == 6 && stage_3){
			if(!rightclick)
				sum_buyer+= 10;
			else
				sum_buyer-= 10;
		}
		else if(button_id == 7 && stage_3){
			if(!rightclick)
				sum_buyer+= 100;
			else
				sum_buyer-= 100;
		}
		else if(button_id >= 11 && button_id <= (11 + max_players) && stage_1){
			int index = button_id - 11;
			if(index >= 0 && index < max_players)
				seller = players[button_id - 11];
		}
		window_proposition->update(sum_seller, sum_buyer);

	}
private:
	Trade_window *window_proposition;
	Trade_window *window_approval;
	Button **buttons;
	int player_draw_y_distance;
	Player *buyer;
	Player *seller;
	int sum_seller, sum_buyer;

	bool stage_1, stage_2, stage_3;
};

#endif