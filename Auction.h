#ifndef AUCTION_H
#define AUCTION_H

#include "Question.h"
#include "Player.h"
#include "Button.h"


class Auction{
public:
	Auction(int pos_x, int pos_y, Player** players, int n_players, ALLEGRO_BITMAP *image, ALLEGRO_FONT *title, ALLEGRO_FONT *normal_text, Street *property_on_sale = 0,bool active = false): pos_x(pos_x), pos_y(pos_y), players(players), n_players(n_players), image(image), ant_buttons(4), title(title), normal_text(normal_text), property_on_sale(property_on_sale), active(active){
		players_bids = new int[n_players];
		buttons = new Button*[ant_buttons];
		buttons[0] = new Button(pos_x + 217, pos_y + 270, pos_x + 217 + 60, pos_y + 270 + 20, 1, "No bid");
		buttons[1] = new Button(pos_x + 378, pos_y + 270, pos_x + 378 + 60, pos_y + 270 + 20, 2, "Bid");
		buttons[2] = new Button(pos_x + 415, pos_y + 170, pos_x + 415 + 23, pos_y + 170 + 26, 3);
		buttons[3] = new Button(pos_x + 415, pos_y + 207, pos_x + 415 + 23, pos_y + 207 + 26, 4);
		window = new Question(pos_x, pos_y, buttons, ant_buttons, "Auction", "Name is on auction and the current bid is sum", image);
	}
	~Auction(){
		delete[] buttons;
		delete[] players_bids;
		delete window;
	}
	void draw(){
		window->draw(title, normal_text);
	}
	void set_property(Street *property_on_sale_in){property_on_sale = property_on_sale_in;}
	void set_active(bool active_in){active = active_in;}
	bool get_active(){return active;}
private:
	ALLEGRO_BITMAP *image;
	ALLEGRO_FONT *title, *normal_text;
	bool active;
	Question* window;
	Player** players;
	Street *property_on_sale;
	int n_players;
	int *players_bids;
	int pos_x, pos_y;
	Button* *buttons;
	int ant_buttons;
};

#endif