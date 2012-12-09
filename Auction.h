#ifndef AUCTION_H
#define AUCTION_H

#include "Question.h"
#include "Player.h"
#include "Button.h"
#include "Text_field.h"


class Auction{
public:
	Auction(int pos_x, int pos_y, Player** players, int n_players, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *button, ALLEGRO_FONT *title, ALLEGRO_FONT *normal_text, Street *property_on_sale = 0,bool active = false);
	~Auction();
	void draw();
	void set_property(Street *property_on_sale_in){property_on_sale = property_on_sale_in;}
	void set_active(bool active_in){
		char temp[100];
		active = active_in;
		property_on_sale->get_namn(temp, 100);
		window->update(temp, current_max_bid, 0, c_player);
	}
	bool get_active(){return active;}
private:
	ALLEGRO_BITMAP *image;
	ALLEGRO_FONT *title, *normal_text;
	bool active;
	Auction_window* window;
	Player** players;
	Street *property_on_sale;
	int n_players, c_player;
	int *players_bids;
	int current_max_bid;
	int pos_x, pos_y;
	Button* *buttons;
	int ant_buttons;
	int max_number(int array_in[], int length);
};

#endif