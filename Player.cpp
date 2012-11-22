#include "Constants.h"

#include "Player.h"
#include "Property.h"

void Player::move_Player(int steg, Property *tomter[]){
	pos_ruta += steg;
	pos_ruta = pos_ruta % ant_rutor;
	update_Player(tomter);
}

void Player::get_pos(int &pos_x_in, int &pos_y_in) const{
	pos_x_in = pos_x;
	pos_y_in = pos_y;
}

void Player::draw_player() const  {
	al_draw_filled_rectangle(pos_x, pos_y, pos_x + 40, pos_y + 40, al_map_rgb(color_r, color_g, color_b));	
}

void Player::update_Player(Property *tomter[]){
	int pos_x_temp, pos_y_temp;
	tomter[pos_ruta]->get_player_pos(pos_x_temp, pos_y_temp);
	pos_x = pos_x_temp;
	pos_y = pos_y_temp;
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

void Player::get_money(int sum_to_get){
	pengar =+ sum_to_get;
}