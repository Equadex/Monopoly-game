#include "Constants.h"

#include "Player.h"
#include "Property.h"

void Player::move_Player(int steg, Property *tomter[], Player **players, int n_player_setoff){
	pos_ruta += steg;
	pos_ruta = pos_ruta % ant_rutor;
	update_Player(tomter, players, n_player_setoff);
}

void Player::get_pos(int &pos_x_in, int &pos_y_in) const{
	pos_x_in = pos_x;
	pos_y_in = pos_y;
}

void Player::draw_player() const  {
	al_draw_filled_rectangle(pos_x, pos_y, pos_x + 40, pos_y + 40, al_map_rgb(color_r, color_g, color_b));	
}

void Player::update_Player(Property *tomter[], Player **players, int n_player_setoff){
	int pos_x_temp, pos_y_temp;
	tomter[pos_ruta]->get_player_pos(pos_x_temp, pos_y_temp);
	pos_x = pos_x_temp;
	pos_y = pos_y_temp;

	if(n_player_setoff > 1){

	}

	if(pos_ruta <= 10)
		pos_y += 10 * (n_player_setoff );
	else if(pos_ruta > 10 && pos_ruta <= 20)
		pos_x -= 10 * (n_player_setoff - id);
	else if(pos_ruta > 20 && pos_ruta <= 30)
		pos_y -= 10 * (n_player_setoff - id);
	else if(pos_ruta > 30 && pos_ruta <= 40)
		pos_x += 10 * (n_player_setoff - id);
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