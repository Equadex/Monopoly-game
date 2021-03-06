#include "Trade.h"

Trade::Trade(int pos_x, int pos_y, Property **tomter, ALLEGRO_FONT *button_text, ALLEGRO_FONT *title_font, ALLEGRO_BITMAP *trade_prop_image, ALLEGRO_BITMAP *trade_reciver_image, ALLEGRO_BITMAP *button_image, ALLEGRO_BITMAP *box_image): stage_1(false), stage_2(false), stage_3(false), player_draw_y_distance(35), seller(0), buyer(0), sum_seller(0), sum_buyer(0),tomter(tomter), n_seller_streets(0), n_buyer_streets(0) {
	const int n_buttons_created = 11 + max_players;
	const int n_buttons2_created = 3;

	buttons = new Button*[n_buttons_created];
	buttons2 = new Button*[n_buttons2_created];

	buttons[0] = new Button(pos_x + 15, pos_y + 120, pos_x + 255, pos_y + 150, -4, "Select player to trade with", 0, true);
	buttons[1] = new Button(pos_x + 280, pos_y + 120, pos_x + 560, pos_y + 150, -3, "Select what you want to trade for", 0, true);
	buttons[2] = new Button(pos_x + 650, pos_y + 120, pos_x + 890, pos_y + 150, -2, "Select what do you want to pay for it", 0, true);
	buttons[3] = new Button(pos_x + 290, pos_y + 455, pos_x + 290 + al_get_bitmap_width(button_image), pos_y + 455 + al_get_bitmap_height(button_image), -1, "Trade", button_image);
	buttons[4] = new Button(pos_x + 530, pos_y + 455, pos_x + 530 + al_get_bitmap_width(button_image), pos_y + 455 + al_get_bitmap_height(button_image), 1, "Cancel", button_image);

	buttons[5] = new Button(pos_x + 270, pos_y + 405, pos_x + 270 + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 2, "1", box_image);
	buttons[6] = new Button(pos_x + 270 + (al_get_bitmap_width(box_image) * 2), pos_y + 405, pos_x + 270 + (al_get_bitmap_width(box_image) * 2) + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 3, "10", box_image);
	buttons[7] = new Button(pos_x + 270 + (al_get_bitmap_width(box_image) * 4), pos_y + 405, pos_x + 270 + (al_get_bitmap_width(box_image) * 4) + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 4, "100", box_image);

	buttons[8] = new Button(pos_x + 635, pos_y + 405, pos_x + 635 + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 5, "1", box_image);
	buttons[9] = new Button(pos_x + 635 + (al_get_bitmap_width(box_image) * 2), pos_y + 405, pos_x + 635 + (al_get_bitmap_width(box_image) * 2) + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 6, "10", box_image);
	buttons[10] = new Button(pos_x + 635 + (al_get_bitmap_width(box_image) * 4), pos_y + 405, pos_x + 635 + (al_get_bitmap_width(box_image) * 4) + al_get_bitmap_width(box_image), pos_y + 405 + al_get_bitmap_height(box_image), 7, "100", box_image);

	buttons2[0] = new Button(pos_x + 182, pos_y + 455, pos_x + 182 + 125, pos_y + 455 + al_get_bitmap_height(button_image), 1, "Accept", 0, true);
	buttons2[1] = new Button(pos_x + 387, pos_y + 455, pos_x + 387 + 125, pos_y + 455 + al_get_bitmap_height(button_image), 2, "Decline", 0, true);
	buttons2[2] = new Button(pos_x + 592, pos_y + 455, pos_x + 592 + 125, pos_y + 455 + al_get_bitmap_height(button_image), 3, "Counter proposal", 0, true);

	for(int i = 0; i < max_players; i++){
		char temp_char[100];
		sprintf(temp_char, "Player %i", i);
		buttons[11 + i] = new Button(pos_x + 40, pos_y + 180 + player_draw_y_distance * i, pos_x + 220, pos_y + 180 + player_draw_y_distance * (i + 1), 11 + i, temp_char, 0, true);
	}


	window_proposition = new Trade_window(pos_x, pos_y, buttons, n_buttons_created, "Trade proposal", "", button_text, title_font, trade_prop_image, tomter, true);
	window_approval = new Trade_window(pos_x, pos_y, buttons2, n_buttons2_created, "Trade proposal", "", button_text, title_font, trade_reciver_image, tomter, false);

	seller_streets = new Street*[ant_rutor];
	buyer_streets = new Street*[ant_rutor];

 	
}

void Trade::draw(){
	if(window_proposition->get_active()){
		if(seller != NULL)
			window_proposition->draw(stage_1, stage_2, stage_3, seller->get_id(), seller_streets, buyer_streets, n_seller_streets, n_buyer_streets);
		else
			window_proposition->draw(stage_1, stage_2, stage_3, -1, seller_streets, buyer_streets, n_seller_streets, n_buyer_streets);
	}
	else if(window_approval->get_active())
		window_approval->draw(stage_1, stage_2, stage_3, seller->get_id(), seller_streets, buyer_streets, n_seller_streets, n_buyer_streets);
}

void Trade::set_active(bool value_in, int current_players){
	if(value_in){
		set_draw_proposition(value_in, current_players);
	}
	else{
		window_proposition->set_active(false);
		window_approval->set_active(false);
	}
}

bool Trade::get_active(){
	return (window_proposition->get_active() + window_approval->get_active());
}

void Trade::set_buyer(Player *buyer_in){
	buyer = buyer_in;
}

void Trade::pressed(int mouse_pos_x, int mouse_pos_y, Player **players, int n_players, bool rightclick){
	int button_id;
	
	if(window_proposition->get_active()){
		button_id = window_proposition->button_pressed(mouse_pos_x, mouse_pos_y);


		if(button_id == -4){
			if(!stage_2 && !stage_3)
			stage_1 = true;
		}
		else if(button_id == -3){
			if((stage_1 || stage_3) && seller != 0){
				stage_1 = false;
				stage_2 = true;
				stage_3 = false;
			}
		}
		else if(button_id == -2){
			if(stage_2){
				stage_1 = false;
				stage_2 = false;
				stage_3 = true;
			}
		}
		else if(button_id == -1){
			if(seller != NULL){
				char temp[200];
				sprintf(temp, "Player %i has suggested this trade", buyer->get_id());
	
				window_proposition->set_active(false);
				stage_1 = false; stage_2 = false; stage_3 = false;
				window_approval->set_text("Trade proposal", temp);
				status_boxes_convert_to_reciever(window_proposition->get_status_buyer_boxes(), n_buyer_streets, window_approval);
				window_approval->set_status_buyer_boxes(window_proposition->get_status_buyer_boxes(), n_buyer_streets);
				status_boxes_convert_to_reciever(window_proposition->get_status_seller_boxes(), n_seller_streets, window_approval, true);
				window_approval->set_status_seller_boxes(window_proposition->get_status_seller_boxes(), n_seller_streets);
				window_approval->update(sum_seller, sum_buyer, seller_streets, buyer_streets, n_seller_streets, n_buyer_streets, seller, buyer); 
				window_approval->set_active(true);
			}
		}
		else if(button_id == 1){
			reset_all_data(n_players);
			set_active(false, 0);
		}
		else if(button_id == 2 && stage_2){
			if(!rightclick){
				if(seller->get_money() >= (sum_seller + 1))
					sum_seller++;
			}
			else
				if((sum_seller - 1) >= 0)
					sum_seller--;
		}
		else if(button_id == 3 && stage_2){
			if(!rightclick){
				if(seller->get_money() >= (sum_seller + 10))
					sum_seller += 10;
			}
			else
				if((sum_seller - 10) >= 0)
				sum_seller -= 10;
		}
		else if(button_id == 4 && stage_2){
			if(!rightclick){
				if(seller->get_money() >= (sum_seller + 100))
					sum_seller += 100;
			}
			else
				if((sum_seller - 100) >= 0)
					sum_seller -= 100;
		}
		else if(button_id == 5 && stage_3){
			if(!rightclick){
				if(buyer->get_money() >= (sum_buyer + 1))
					sum_buyer++;
			}
			else
				if((sum_seller - 1) >= 0)
				sum_buyer--;
		}
		else if(button_id == 6 && stage_3){
			if(!rightclick){
				if(buyer->get_money() >= (sum_buyer + 10))
					sum_buyer+= 10;
			}
			else
				if((sum_buyer - 10) >= 0)
					sum_buyer-= 10;
		}
		else if(button_id == 7 && stage_3){
			if(!rightclick){
				if(buyer->get_money() >= (sum_buyer + 100))
					sum_buyer+= 100;
			}
			else
				if((sum_buyer - 100) >= 0)
					sum_buyer-= 100;
		}
		else if(button_id >= 11 && button_id <= (11 + max_players) && stage_1){
			int index = button_id - 11; //index is the same as players id
			int index2;
			if(index >= 0 && index < max_players && (buyer->get_id() != index)){
				for(int i = 0; i < n_players; i++){
					if(players[i]->get_id() == index)
						index2 = i;
				}
				
				seller = players[index2];
			}
		}
	}
	else if(window_approval->get_active()){
		button_id = window_approval->button_pressed(mouse_pos_x, mouse_pos_y);

		if(button_id == 1){
			make_trade();
			reset_all_data(n_players);
			set_active(false, 0);
		}
		else if(button_id == 2){
			reset_all_data(n_players);
			set_active(false, 0);
		}
		else if(button_id == 3){
			window_approval->set_active(false);
			window_proposition->set_active(true);
			switch_buyer_seller();
		}
	}

	//Checks if any status_boxes was pressed
	bool found = false;
	for(int i = 0; i < ant_rutor && !found; i++){
		if(tomter[i]->get_typ() == TOMT){
			if(((Street*)tomter[i])->button_pressed(mouse_pos_x, mouse_pos_y)){
				bool already_added = false;
				for(int j = 0; j < n_seller_streets; j++){
					if(seller_streets[j]->get_pos_ruta() == ((Street*)tomter[i])->get_pos_ruta())
						already_added = true;
				}
				for(int j = 0; j < n_buyer_streets; j++){
					if(buyer_streets[j]->get_pos_ruta() == ((Street*)tomter[i])->get_pos_ruta())
						already_added = true;
				}
				if(stage_2 && !already_added){
					if(((Street*)tomter[i])->get_Owner() == seller && ((Street*)tomter[i])->get_Owner() != 0){
						seller_streets[n_seller_streets++] = ((Street*)tomter[i]);
						found = true;
					}
				}
				else if(stage_3 && !already_added)
					if(((Street*)tomter[i])->get_Owner() == buyer){
						buyer_streets[n_buyer_streets++] = ((Street*)tomter[i]);
						found = true;
					}
			}
		}
	}

	//Checks if any of windows status_boxes was pressed

	for(int i = 0; i < n_buyer_streets; i++){
		Status_box *temp = window_proposition->get_status_buyer_boxes();
		
		if(temp[i].pressed(mouse_pos_x, mouse_pos_y)){
			if(((temp[i].get_street())->get_Street_info())->get_active())
				((temp[i].get_street())->get_Street_info())->set_active(false);
			else{
				al_set_street_info_false(tomter);
				((temp[i].get_street())->get_Street_info())->set_active(true);
			}
		}

			
	}

	for(int i = 0; i < n_seller_streets; i++){
		Status_box *temp = window_proposition->get_status_seller_boxes();
			if(temp[i].pressed(mouse_pos_x, mouse_pos_y)){
				if(((temp[i].get_street())->get_Street_info())->get_active())
					((temp[i].get_street())->get_Street_info())->set_active(false);
				else{
					al_set_street_info_false(tomter);
					((temp[i].get_street())->get_Street_info())->set_active(true);
				}
			}
		}

	if(window_proposition->get_active())
		window_proposition->update(sum_seller, sum_buyer, seller_streets, buyer_streets, n_seller_streets, n_buyer_streets, seller, buyer);

}

void Trade::al_set_street_info_false(bool full){ //Full = true resets all, false only the first
	for(int i = 0; i < ant_rutor; i++){
		if(tomter[i]->get_typ()  == TOMT){ // Om tomten �r av gatutyp
			if((((Street*)tomter[i])->get_Street_info())->get_active()){
				(((Street*)tomter[i])->get_Street_info())->set_active(false);
				if(!full)
					break;
			}
		}
	}
}

void Trade::status_boxes_convert_to_reciever(Status_box *boxes, int n_boxes, Trade_window *window, bool seller){
	const int x_displacement = 40, y_displacement = 40, widht = 30, height = 30 ,pos_x_temp = window->get_pos_x(), pos_y_temp = window->get_pos_y(), y_max = 8, x_max = 10;
	int x_start = 45, y_start = 150, y = 0, x = 0;
	if(!seller)
		x_start = 490;
	
	for(int i = 0; i < n_boxes; i++, x++){
		if(i >= x_max){
			if(y < y_max)
				y++;
			x = 0;
		}
		
		boxes[i].set_coordinates(pos_x_temp + x_start + x * x_displacement, pos_y_temp + y_start + y * y_displacement, pos_x_temp + x_start + x * x_displacement + widht, pos_y_temp + y_start + y * y_displacement + height);
	}
}

void Trade::reset_all_data(int n_players){
	for(int i = 0; i < n_seller_streets; i++){
		seller_streets[i] = 0;
	}
	n_seller_streets = 0;
	for(int i = 0; i < n_buyer_streets; i++){
		buyer_streets[i] = 0;
	}

	window_proposition->set_n_buttons(window_proposition->get_n_buttons() + max_players - n_players);

	n_buyer_streets = 0;
	buyer = 0;
	seller = 0;
	sum_seller = 0;
	sum_buyer = 0;
	stage_1 = stage_2 = stage_3 = false;
}	

void Trade::set_draw_proposition(bool value_in, int current_players){
	window_proposition->set_n_buttons(window_proposition->get_n_buttons() - max_players + current_players);
	window_proposition->set_active(value_in);
	window_proposition->update(sum_seller, sum_buyer, seller_streets, buyer_streets, n_seller_streets, n_buyer_streets, seller, buyer);
}

void Trade::make_trade(){
	for(int i = 0; i < n_seller_streets; i++){
		seller_streets[i]->buy_Street(buyer, true, 0);
	}
	for(int i = 0; i < n_buyer_streets; i++){
		buyer_streets[i]->buy_Street(seller, true, 0);
	}
	buyer->pay_player(seller, sum_buyer);
	seller->pay_player(buyer, sum_seller);
}

void Trade::update_trade_buttons(Player **players, int n_players){
	for(int i = 0; i < n_players; i++){
		char temp_char[100];
		sprintf(temp_char, "Player %i", players[i]->get_id());
		buttons[11 + i]->set_ID(11 + players[i]->get_id());
		buttons[11 + i]->set_label(temp_char);
	}
	int n_buttons_temp = window_proposition->get_n_buttons() - 11;
	if(n_buttons_temp > n_players){
		for(int i = 0; i < (n_buttons_temp - n_players); i++){
			buttons[11 + max_players - 1 - i]->set_active(false);
		}
	}
}