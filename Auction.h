#ifndef AUCTION_H
#define AUCTION_H

#include "Button.h" //Must be here
#include "Text_field.h" //Must be here
#include "Window_list.h" //Must be here
#include "Auction_window.h" //Must be here, will include base class Question
#include "Player.h" //Is used to optain an players money
#include "Street.h" //Is used to get street name


class Auction{
public:
	Auction(int pos_x, int pos_y, Player** players, int &n_players, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *button, ALLEGRO_BITMAP *box, ALLEGRO_FONT *title, ALLEGRO_FONT *normal_text, Street *property_on_sale = 0,bool active = false);
	~Auction();
	void draw();
	void set_property(Street *property_on_sale_in){property_on_sale = property_on_sale_in;}
	void set_active(bool active_in);
	bool get_active(){return active;}
	void button_pressed(int mouse_x, int mouse_y, bool right_click = false);
private:
	ALLEGRO_BITMAP *image;
	ALLEGRO_FONT *title, *normal_text;
	bool active;
	bool *no_bid;
	Auction_window* window;
	Window_list *player_bid_list;
	Player** players;
	Street *property_on_sale;
	int c_player, current_bid;
	int &n_players;
	int *players_bids;
	int current_max_bid;
	int pos_x, pos_y;
	Button* *buttons;
	int ant_buttons;
	int max_number(int array_in[], int length, int *n);
	void clr_player_data();
};

#endif