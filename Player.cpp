#include "Constants.h"

#include "Player.h"
#include "Property.h"

void Player::move_Player(int steg){
	pos_ruta += steg;
	pos_ruta = pos_ruta % ant_rutor;
	//update_Player(tomter, player_IDs, n_player_setoff);
}

void Player::get_pos(int &pos_x_in, int &pos_y_in) const{
	pos_x_in = pos_x;
	pos_y_in = pos_y;
}

void Player::draw_player() const  {
	al_draw_filled_rectangle(pos_x, pos_y, pos_x + square_size, pos_y + square_size, al_map_rgb(color_r, color_g, color_b));	
}

void Player::update_Player(Property *tomter[], Player **players, int *player_IDs, int n_player_setoff, bool first_update){ 
	int new_pos_x, new_pos_y;
	int temp_pos_x, temp_pos_y;
	int displacement_factor;
	tomter[pos_ruta]->get_player_pos(new_pos_x, new_pos_y);

	//Centrerar spelaren på positionen

	new_pos_x -= square_size / 2;
	new_pos_y -= square_size / 2;

	//Konstant förskutning 

	if(pos_ruta == 0){
		new_pos_y -= 20;
	}
	else if(pos_ruta > 10 && pos_ruta <= 20)
		new_pos_x -= 10;
	else if(pos_ruta > 20 && pos_ruta <= 30)
		new_pos_y -= 10;

	if(first_update){//Första gången endast
		
		if(pos_ruta == 0)
			new_pos_y += 10 * id;
		else if(pos_ruta <= 10)
			new_pos_y += 10 * id;
		else if(pos_ruta > 10 && pos_ruta <= 20)
			new_pos_x -= 10 * id;
		else if(pos_ruta > 20 && pos_ruta <= 30)
			new_pos_y -= 10 * id;
		else if(pos_ruta > 30 && pos_ruta <= 40)
			new_pos_x += 10 * id;
	
	}
	else{
		//Beräknar hur förskjuten spelaren ska vara på rutan

		for(int i = 0; i < n_player_setoff; i++){
			bool loop_done = false;
			players[player_IDs[i]]->get_pos(temp_pos_x, temp_pos_y);
			while(!loop_done){
				if(temp_pos_x == new_pos_x && temp_pos_y == new_pos_y){
					if(pos_ruta == 0){
						new_pos_y += 10;
					}
					else if(pos_ruta <= 10)
						new_pos_y += 10;
					else if(pos_ruta > 10 && pos_ruta <= 20)
						new_pos_x -= 10;
					else if(pos_ruta > 20 && pos_ruta <= 30)
						new_pos_y -= 10;
					else if(pos_ruta > 30 && pos_ruta <= 40)
						new_pos_x += 10;
				}
				else{
					loop_done = true;
				}
			}
		}
	}

	pos_x = new_pos_x;
	pos_y = new_pos_y;
}

bool Player::pay(int sum_to_pay){
	if(sum_to_pay <= pengar){
		pengar -= sum_to_pay;
		return (true);
	}
	else{
		return (false);
	}
}

void Player::recieve_money(int sum_to_get){
	pengar += sum_to_get;
}

int Player::get_money() const{
	return (pengar);
}

void Player::get_color(int *array_in) const{
	array_in[0] = color_r;
	array_in[1] = color_g;
	array_in[2] = color_b;
}

void Player::defeated(Player *winner, Property* tomter[]){
	winner->recieve_money(pengar);
	for(int i = 0; i < ant_rutor; i++){
		if(tomter[i]->get_Owner() == this){
			tomter[i]->set_Owner(winner);
		}
	}
}