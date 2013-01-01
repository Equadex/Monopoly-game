#include "Auction.h"

Auction::Auction(int pos_x, int pos_y, Player** players, int n_players, ALLEGRO_BITMAP *image, ALLEGRO_BITMAP *button, ALLEGRO_BITMAP *box, ALLEGRO_FONT *title, ALLEGRO_FONT *normal_text, Street *property_on_sale,bool active) : pos_x(pos_x), pos_y(pos_y), players(players), n_players(n_players), image(image), ant_buttons(9), title(title), normal_text(normal_text), property_on_sale(property_on_sale), active(active), current_max_bid(0), c_player(0), current_bid(0){
	const int ant_text_fields = 3;
	Text_field** temp_text = new Text_field*[ant_text_fields];
	
	players_bids = new int[n_players];
	no_bid = new bool[n_players];

	for(int i = 0; i < n_players; i++){
		players_bids[i] = 0;
		no_bid[i] = false;
	}

	buttons = new Button*[ant_buttons];
	buttons[0] = new Button(pos_x + 450 - 30 - 80, pos_y + 400, pos_x + 450 - 30, pos_y + 400 + 25, 1, "No bid", button);
	buttons[1] = new Button(pos_x + 450 + 30, pos_y + 400, pos_x + 450 + 30 + 80, pos_y + 400 + 25, 2, "Bid", button);
	buttons[2] = new Button(pos_x + 565, pos_y + 255, pos_x + 565 + 30, pos_y + 255 + 30, 3, "1", box);
	buttons[3] = new Button(pos_x + 565 + 50, pos_y + 255, pos_x + 565 + 80, pos_y + 255 + 30, 4, "5", box);
	buttons[4] = new Button(pos_x + 665, pos_y + 255, pos_x + 665 + 30, pos_y + 255 + 30, 5, "10", box);
	buttons[5] = new Button(pos_x + 665 + 50, pos_y + 255, pos_x + 665 + 80, pos_y + 255 + 30, 6, "50", box);
	buttons[6] = new Button(pos_x + 765, pos_y + 255, pos_x + 765 + 30, pos_y + 255 + 30, 7, "100", box);
	buttons[7] = new Button(pos_x + 765 + 50, pos_y + 255, pos_x + 765 + 80, pos_y + 255 + 30, 8, "500", box);
	buttons[8] = new Button(pos_x + 865, pos_y + 255, pos_x + 865 + 30, pos_y + 255 + 30, 9, "1000", box);

	temp_text = new Text_field*[ant_text_fields];

	temp_text[0] = new Text_field(300, 240,"%s is on auction and the current bid is %i" , normal_text);
	temp_text[1] = new Text_field(340, 380,"Your bid: %i", normal_text);
	temp_text[2] = new Text_field(20, 175, "Player %i", normal_text);

	window = new Auction_window(pos_x, pos_y, buttons, ant_buttons, "Auction", "", image,temp_text, ant_text_fields, true);
	player_bid_list = new Window_list(pos_x + 50, pos_y + 120, players_bids, players, n_players, 0, true);
}

Auction::~Auction(){
	delete[] buttons;
	delete[] players_bids;
	delete window;
}

void Auction::draw(){

	window->draw(title, normal_text);
	player_bid_list->draw(title, normal_text);
}

int Auction::max_number(int array_in[], int length, int *n){
	int max_n = array_in[0];
	n[0] = 0;

	for(int i = 1; i < length; i++){
		if(max_n < array_in[i]){
			max_n = array_in[i];
			n[0] = i;
		}
	}
	return(max_n);
}

void Auction::set_active(bool active_in){
	char temp[100];
	active = active_in;
	if(active_in == true)
		property_on_sale->get_namn(temp, 100);
	window->set_active(active_in);
	window->update(temp, current_max_bid, current_max_bid + 1, c_player);
}

void Auction::button_pressed(int mouse_x, int mouse_y){
	int button_pressed = window->button_pressed(mouse_x, mouse_y);
	if(current_bid == 0)
		current_bid = current_max_bid + 1;

	switch(button_pressed){
		case 1:
			no_bid[c_player] = true;
			player_bid_list->update_status(false, players[c_player]);
			current_bid = current_max_bid + 1;
			c_player = (c_player + 1) % n_players;
			break;
		case 2:
			players_bids[c_player] = current_bid;
			player_bid_list->update_status(true, players[c_player]);
			current_max_bid = current_bid;
			current_bid = current_max_bid + 1;
			c_player = (c_player + 1) % n_players;
			break;
		case 3:
			if((current_bid + 1) < players[c_player]->get_money())
				current_bid++;
			break;
		case 4:
			if((current_bid + 5) < players[c_player]->get_money())
				current_bid = current_bid + 5;
			break;
		case 5:
			if((current_bid + 10) < players[c_player]->get_money())
				current_bid = current_bid + 10;
			break;
		case 6:
			if((current_bid + 50) < players[c_player]->get_money())
				current_bid = current_bid + 50;
			break;
		case 7:
			if((current_bid + 100) < players[c_player]->get_money())
				current_bid = current_bid + 100;
			break;
		case 8:
			if((current_bid + 500) < players[c_player]->get_money())
				current_bid = current_bid + 500;
			break;
		case 9:
			if((current_bid + 1000) < players[c_player]->get_money())
				current_bid = current_bid + 1000;
			break;
	}
	
	char temp[100];
	property_on_sale->get_namn(temp, 100);
	window->update(temp, current_max_bid, current_bid, c_player);
	
	int folders = 0;
	for(int i = 0; i < n_players; i++){
		if(no_bid[i] == true){
			folders++;
		}
	}
	if(folders == n_players){
		window->set_active(false);
		set_active(false);
		folders = 0;

		int sum;
		int n[1];

		sum = max_number(players_bids, n_players, n);
		if(sum > 0 && players[n[0]]->get_money() >= sum)
			property_on_sale->buy_Street(players[n[0]], false, sum);
		clr_player_data();
	}
}

void Auction::clr_player_data(){
	for(int i = 0; i < n_players; i++){
		players_bids[i] = 0;
		no_bid[i] = false;
		current_max_bid = 0;
		current_bid = current_max_bid + 1;
		c_player = 0;
	}
}