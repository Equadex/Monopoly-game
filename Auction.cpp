#include "Auction.h"

Auction::Auction(int pos_x, int pos_y, Player** players, int n_players, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *button, ALLEGRO_FONT *title, ALLEGRO_FONT *normal_text, Street *property_on_sale,bool active) : pos_x(pos_x), pos_y(pos_y), players(players), n_players(n_players), image(image), ant_buttons(4), title(title), normal_text(normal_text), property_on_sale(property_on_sale), active(active), current_max_bid(0), c_player(0){
	const int ant_text_fields = 3;
	Text_field** temp_text = new Text_field*[ant_text_fields];
	
	players_bids = new int[n_players];
	buttons = new Button*[ant_buttons];
	buttons[0] = new Button(pos_x + 450 - 30 - 80, pos_y + 400, pos_x + 450 - 30, pos_y + 400 + 25, 1, "No bid", button);
	buttons[1] = new Button(pos_x + 450 + 30, pos_y + 400, pos_x + 450 + 30 + 80, pos_y + 400 + 25, 2, "Bid", button);
	buttons[2] = new Button(pos_x + 575, pos_y + 234, pos_x + 575 + 30, pos_y + 234 + 38, 3);
	buttons[3] = new Button(pos_x + 575, pos_y + 285, pos_x + 575 + 30, pos_y + 285 + 38, 4);
	

	temp_text = new Text_field*[ant_text_fields];

	temp_text[0] = new Text_field(300, 240,"%s is on auction and the current bid is %i" , normal_text);
	temp_text[1] = new Text_field(340, 380,"Your bid: %i", normal_text);
	temp_text[2] = new Text_field(20, 175, "Player %i", normal_text);

	window = new Auction_window(pos_x, pos_y, buttons, ant_buttons, "Auction", "", image,temp_text, ant_text_fields);
}

Auction::~Auction(){
	delete[] buttons;
	delete[] players_bids;
	delete window;
}

void Auction::draw(){

	window->draw(title, normal_text);
}

int Auction::max_number(int array_in[], int length){
	int max_n = array_in[0];
	
	for(int i = 1; i < length; i++){
		if(max_n < array_in[i]){
			max_n = array_in[i];
		}
	}
	return(max_n);
}