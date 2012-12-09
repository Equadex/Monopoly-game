#ifndef AUCTION_H
#define AUCTION_H

#include "Question.h"
#include "Player.h"


class Auction{
public:
	Auction(int pos_x, int pos_y, Player* players, int n_players, ALLEGRO_BITMAP *image): pos_x(pos_x), pos_y(pos_y), players(players), n_players(n_players), image(image), ant_buttons(4){
		players_bids = new int[n_players];
		buttons = new (Button*)[ant_buttons];
		buttons[0] = new Button(pos_x + 217, pos_y + 270, pos_x + 217 + 60, pos_y + 270 + 20,	1, "No bid");
		buttons[1] = new Button(pos_x + 377, pos_y + 270, pos_x + 277 + 60, pos_y + 270 + 20, 2, "Bid");
		buttons[2] = new Button(pos_x + 415, pos_y + 170, pos_x + 415 + 25, pos_y + 170 + 27, 3);
		buttons[3] = new Button(pos_x + 415, pos_y + 207, pos_x + 415 + 25, pos_y + 207 + 27, 4);
	}
private:
	ALLEGRO_BITMAP *image;
	Question* window;
	Player* players;
	Street *property_on_sale;
	int n_players;
	int *players_bids;
	int pos_x, pos_y;
	Button** buttons;
	int ant_buttons;
}

#endif