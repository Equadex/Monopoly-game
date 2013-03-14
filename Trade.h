#ifndef TRADE_H
#define TRADE_H

#include <allegro5/allegro.h>
//#include <allegro5\allegro_image.h>

#include "Trade_window.h"
#include "Button.h"
#include "Player.h"
#include "Status_box.h"
#include "Street_info.h"


class Trade{
public:
	Trade(int pos_x, int pos_y, Property **tomter, ALLEGRO_FONT *button_text, ALLEGRO_FONT *title_font, ALLEGRO_BITMAP *trade_prop_image, ALLEGRO_BITMAP *trade_reciver_image, ALLEGRO_BITMAP *button_image, ALLEGRO_BITMAP *box_image);

	void draw();
	void set_active(bool value_in, int current_players);
	bool get_active();
	void set_buyer(Player *buyer_in);
	void pressed(int mouse_pos_x, int mouse_pos_y, Player **players, int n_players, bool rightclick = false);
private:
	Trade_window *window_proposition;
	Trade_window *window_approval;
	Button **buttons;
	Button **buttons2;
	Property **tomter;
	Street **seller_streets;
	Street **buyer_streets;
	int n_seller_streets, n_buyer_streets;
	const int player_draw_y_distance;
	Player *buyer;
	Player *seller;
	int sum_seller, sum_buyer;

	bool stage_1, stage_2, stage_3;

	void al_set_street_info_false(bool full = false); //Full = true resets all, false only the first
	void status_boxes_convert_to_reciever(Status_box *boxes, int n_boxes, Trade_window *window, bool seller = false);
	void reset_all_data(int n_players);
	void set_draw_proposition(bool value_in, int current_players);
	void make_trade();

};

#endif